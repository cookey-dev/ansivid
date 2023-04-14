const Jimp = require('jimp');
const ora = require('ora');
const fs = require('fs-extra');
const { execSync } = require('child_process');

var spin;

var fps = execSync(
	`ffprobe -v quiet -select_streams v -of default=noprint_wrappers=1:nokey=1 -show_entries stream=r_frame_rate ${process.argv[2]}`
	).toString('utf-8').split('/');
fps = Math.round(parseInt(fps[0]) / parseInt(fps[1]));
console.log('FPS: ' + fps);

if (!fs.existsSync('./frames')) fs.mkdirSync('./frames');
else {
	fs.emptyDirSync('./frames');
}

spin = ora('Splitting video into frames').start();
execSync(`ffmpeg -v error -i ${process.argv[2]} frames/f-%03d.jpg`)
spin.succeed();
const frames = fs.readdirSync('./frames');
fs.writeFileSync('out.json', '{"data":[]}');
var frNum = 0;

(async () => {
	spin = ora('Loading frame 0').start();
	for (var frame of frames) {
		frNum++;
		spin.text = 'Loading frame ' + frNum;
		var img = await Jimp.read('frames/' + frame);
		img = img.resize(process.stdout.columns, process.stdout.rows);
		var pxarr = [];
		var pxproc = 0;
		for (var y = 0; y < process.stdout.rows; y++) {
			for (var x = 0; x < process.stdout.columns; x++) {
				pxproc++;
				var pxcol = Jimp.intToRGBA(img.getPixelColor(x, y));
				pxarr.push(`${"\033"}[38;2;${pxcol.r};${pxcol.g};${pxcol.b}m█`)
				pxcol = null;
			}
			pxarr.push('\n');
		}
		pxarr.push("\033[0m");
		var content = fs.readFileSync('out.json', 'utf8');
		content = JSON.parse(content);
		content.data.push(pxarr);
		fs.writeFileSync('out.json', JSON.stringify(content));
		pxarr = null;
		content = null;
		img = null;
	}
	spin.succeed();
})();
