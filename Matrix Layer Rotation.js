function valueAt(matrix, x, y, rot) {
    var level, limits = { x:0, y:0 }, levels = { x: 0, y: 0};
    levels.x = (x < matrix[y].length/2) ? x : matrix[y].length - x - 1;
    levels.y = (y < matrix.length/2) ? y : matrix.length - y - 1;
    level = Math.min(levels.x, levels.y);
    limits.x = matrix[y].length-level-1;
    limits.y = matrix.length-level-1;
    
    var max = (matrix.length-level*2)*2 + (matrix[y].length-level*2)*2 - 4;
    //process.stdout.write("\n! " + max + " : " + rot + " -x " + limits.x + " -y " + limits.y + "\n");

    rot = rot % max;
    
    
    for (; rot > 0; rot--)
        if (y == level && x < limits.x) x += 1;
        else if (x == limits.x && y < limits.y) y += 1;
        else if (y == limits.y && x > level) x -= 1;
        else if (x == level && y > level) y-=1;
    
    return matrix[y][x];
}

function processData(input) {
    var numbers, lines = input.split("\n");
    var dims = lines[0].split(' ').map(function(v) { return parseInt(v); });
    var rows = dims[0];
    var cols = dims[1];
    var rots = dims[2];
    var matrix = [];
    for (var y = 0; y < lines.length-1; y++) {
        numbers = lines[y+1].split(' ');
        matrix.push(numbers);
        //for (var x = 0; x < numbers.length; )
    }
    
    for (var y = 0; y < matrix.length; y++) {
        for (var x = 0; x < matrix[y].length; x++)
            process.stdout.write(valueAt(matrix, x, y, rots) + " ");
        process.stdout.write("\n");
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
