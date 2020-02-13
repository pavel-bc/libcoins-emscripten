function toHexString(byteArray) {
    return Array.prototype.map.call(byteArray, function(byte) {
        return ('0' + (byte & 0xFF).toString(16)).slice(-2);
    }).join('');
}

function toByteArray(hexString) {
    var result = [];
    for (var i = 0; i < hexString.length; i += 2) {
        result.push(parseInt(hexString.substr(i, 2), 16));
    }
    return result;
}

function toUint8Array(vector, size) {
    var arr = new Uint8Array(size);
    for (var i = 0; i < size; i++) {
        arr[i] = vector.get(i);
    }
    return arr;
}

function printHTML(string) {
    document.writeln('<pre>' + string + '</pre>');
}