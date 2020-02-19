#include <stdio.h>
#include <string>
#include <vector>

#include <pb_encode.h>
#include <pb_decode.h>

#include "proto/libcoins.pb.h"
#include "TrezorCrypto/bip32.h"
#include "TrezorCrypto/curves.h"
#include "TrezorCrypto/secp256k1.h"

#include "HDWallet.h"

namespace libcoins {
    int HDWallet::hdnode_new(const std::vector<uint8_t> & seed, std::vector<uint8_t> & result) {
        // Generate key
        HDNode cNode;
        hdnode_from_seed(seed.data(), seed.size(), SECP256K1_NAME, &cNode);
        hdnode_fill_public_key(&cNode);

        uint8_t buffer[512];
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        HDNodeMessage hdnode = HDWallet::hdnode_to_message(&cNode);    

        // Encode to binary
        pb_encode(&stream, HDNodeMessage_fields, &hdnode);

        // Copy to output destination
        result.assign(std::begin(buffer), std::end(buffer));
        return stream.bytes_written;
    }

    std::string HDWallet::serialize_xprv(std::vector<uint8_t> hdNode) {
        // Decode from hex
        uint8_t* buffer = hdNode.data();
        pb_istream_t stream = pb_istream_from_buffer(buffer, hdNode.size());

        // Decode from binary
        HDNodeMessage message = HDNodeMessage_init_zero;
        pb_decode(&stream, HDNodeMessage_fields, &message);

        // Convert `HDNodeMessage` to `HDNode`
        HDNode myNode = HDWallet::hdnode_to_native(&message);

        // Serialize
        char result[112];
        hdnode_serialize_private(&myNode, 0, VERSION_PRIVATE, result, sizeof(result));
        printf("[C] xprv: %s\n", result);

        std::string output(result);
        return output;
    }

    // HDNode (C) -> HDNodeMessage (Protobuf)
    HDNodeMessage HDWallet::hdnode_to_message(HDNode* cNode) {
        // Construct `BigNum`s from `HDNode`
        BigNum prime = BigNum_init_zero;
        prime.val_count = 9;

        BigNum order = BigNum_init_zero;
        order.val_count = 9;

        BigNum order_half = BigNum_init_zero;
        order_half.val_count = 9;

        BigNum b = BigNum_init_zero;
        b.val_count = 9;

        BigNum Gx = BigNum_init_zero;
        Gx.val_count = 9;

        BigNum Gy = BigNum_init_zero;
        Gy.val_count = 9;

        // Copy
        for (int i = 0; i < 9; i++) {
            prime.val[i] = cNode->curve->params->prime.val[i];
            Gx.val[i] = cNode->curve->params->G.x.val[i];
            Gy.val[i] = cNode->curve->params->G.y.val[i];
            order.val[i] = cNode->curve->params->order.val[i];
            order_half.val[i] = cNode->curve->params->order_half.val[i];
            b.val[i] = cNode->curve->params->b.val[i];
        }

        // Construct `CurvePoint` from `HDNode`
        CurvePoint generator = CurvePoint_init_zero;
        generator.x = Gx;
        generator.y = Gy;

        // Construct `CurveParams` from `HDNode`
        CurveParams params = CurveParams_init_zero;
        params.prime = prime;
        params.g = generator;
        params.order = order;
        params.order_half = order_half;
        params.value = cNode->curve->params->a;
        params.b = b;

        // Construct `CurveInfo` from `HDNode`
        CurveInfo curve = CurveInfo_init_zero;
        curve.params = params;
        curve.hasher_base58 = CurveInfo_HasherType_HASHER_SHA2D;
        curve.hasher_sign = CurveInfo_HasherType_HASHER_SHA2D;
        curve.hasher_pubkey = CurveInfo_HasherType_HASHER_SHA2_RIPEMD;
        curve.hasher_script = CurveInfo_HasherType_HASHER_SHA2;

        char* bip32_name = "Bitcoin seed";
        memcpy(curve.bip32_name, bip32_name, 12);

        // Construct `HDNodeMessage` from `HDNode`
        HDNodeMessage_chain_code_t chain_code = { .size = 32 };
        memcpy(chain_code.bytes, cNode->chain_code, chain_code.size);

        HDNodeMessage_private_key_t private_key = { .size = 32 };
        memcpy(private_key.bytes, cNode->private_key, private_key.size);

        HDNodeMessage_private_key_extension_t private_key_extension = { .size = 32 };
        memcpy(private_key_extension.bytes, cNode->private_key_extension, private_key_extension.size);

        HDNodeMessage_public_key_t public_key = { .size = 33 };
        memcpy(public_key.bytes, cNode->public_key, public_key.size);

        HDNodeMessage hdnode = HDNodeMessage_init_zero;
        hdnode.depth = cNode->depth;
        hdnode.child_num = cNode->child_num;
        hdnode.chain_code = chain_code;
        hdnode.private_key = private_key;
        hdnode.private_key_extension = private_key_extension;
        hdnode.public_key = public_key;
        hdnode.curve = curve;

        return hdnode;
    }

    // HDNodeMessage (Protobuf) -> HDNode (C)
    HDNode HDWallet::hdnode_to_native(HDNodeMessage* message) {
        HDNode node = HDNode {
            .depth = message->depth,
            .child_num = message->child_num,
            .curve = get_curve_by_name(SECP256K1_NAME) // lazy shortcut
        };

        memcpy(node.chain_code, message->chain_code.bytes, message->chain_code.size);
        memcpy(node.private_key, message->private_key.bytes, message->private_key.size);
        memcpy(node.private_key_extension, message->private_key_extension.bytes, message->private_key_extension.size);
        memcpy(node.public_key, message->public_key.bytes, message->public_key.size);
        return node;
    }    
}