import * as Process from "child_process"

Process.exec(
    'reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SoftwareProtectionPlatform\" /v BackupProductKeyDefault',
    (_error, stdout) => {
        const regex = /[\dA-Z]+-[\dA-Z]+-[\dA-Z]+-[\dA-Z]+-[\dA-Z]+/gi
        const productKey: string = (stdout.match(regex) || ['XXXXX-XXXXX-XXXXX-XXXXX-XXXXX'])[0]
        console.log('Product key: ', productKey)
        console.log('Press any key to exit...')
    }
);

process.stdin.setRawMode(true)
process.stdin.resume()
process.stdin.on('data', process.exit.bind(process, 0))
