const exec = require('child_process').exec;

exec('reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SoftwareProtectionPlatform\" /v BackupProductKeyDefault', (error, stdout, stderr) => {
    console.log('Product key: ', stdout.match(/[\dA-Z]+-[\dA-Z]+-[\dA-Z]+-[\dA-Z]+-[\dA-Z]+/i)[0]);
    console.log('Press any key to exit');
});

process.stdin.setRawMode(true);
process.stdin.resume();
process.stdin.on('data', process.exit.bind(process, 0));
