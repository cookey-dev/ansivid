const fs = require('fs-extra');
var content = fs.readFileSync('out.txt', 'utf8');
content = content.split('nf');
var i = 0;
setInterval(() => {
    if (!content[i]) {
        if (!process.argv[2]) {
            console.log('\033[0m');
            process.exit(0);
        } else i = 0;
    }
    var current = content[i].replace(/\n\n/g, "\n")
    console.log(current.trim());
    i++;
}, 1000 / parseInt(fs.readFileSync('fps.txt', 'utf8')));
