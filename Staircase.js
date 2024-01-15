

function processData(input) {
    
    var steps = parseInt(input);
    
    for (var i = 0; i++ < steps; ) {
        
        var line = '';
        var spaces = steps - i;
        
        for (var j = spaces; j--; ) {
            line += ' ';
        }
        
        for (var j = i; j--; ) {
            line += '#';
        }
        
        console.log(line);
    }
} 

process.stdin.resume();
process.stdin.setEncoding("ascii");
_input = "";
process.stdin.on("data", function (input) {
    _input += input;
});

process.stdin.on("end", function () {
   processData(_input);
});
