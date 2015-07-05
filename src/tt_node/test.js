// process.stdin.setEncoding('utf8');

// process.stdin.on('readable', function() {
//   var chunk = process.stdin.read();
//   if (chunk !== null) {
//     process.stdout.write('data: ' + chunk);
//   }
// });


// process.stdin.setRawMode( true );

// process.stdin.on('data', function(key) {
//   // ctrl-c ( end of text )
//   if ( key === '\u0003' ) {
//     process.exit();
//   }
//   // write the key to stdout all normal like
//   process.stdout.write( key+' shit\n' );
// });

var b = new Buffer(6);
b.writeUInt16BE(0x90ab, 0);
b.writeUInt32BE(0x345678, 2);
var c = b.readUIntBE(0, 4).toString(16);  // Specify 6 bytes (48 bits)
console.log('--%s--', c);






