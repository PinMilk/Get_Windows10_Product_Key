/**
 * Windows 10/11 레지스트리에 저장된 백업 제품 키를 조회합니다.
 * 실행: tsc로 컴파일한 뒤 Windows 터미널에서 node productKey.js
 * 조회한 값이 현재 사용 중인 라이선스나 정품 인증 상태를 보장하지는 않습니다.
 */
import * as Process from "child_process"

// Windows의 reg 명령을 비동기로 실행하여 BackupProductKeyDefault 값을 읽습니다.
Process.exec(
    'reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SoftwareProtectionPlatform\" /v BackupProductKeyDefault',
    (_error, stdout) => {
        // 영문자·숫자로 이루어진 다섯 그룹이 하이픈으로 연결된 첫 번째 값을 추출합니다.
        const regex = /[\dA-Z]+-[\dA-Z]+-[\dA-Z]+-[\dA-Z]+-[\dA-Z]+/gi
        // 조회 실패 또는 일치하는 값이 없으면 기본 표시값을 사용합니다.
        const productKey: string = (stdout.match(regex) || ['XXXXX-XXXXX-XXXXX-XXXXX-XXXXX'])[0]
        console.log('Product key: ', productKey)
        console.log('Press any key to exit...')
    }
);

// Enter 없이 키 입력을 받습니다. setRawMode는 대화형 터미널(TTY)에서 사용할 수 있습니다.
process.stdin.setRawMode(true)
process.stdin.resume()
// 키 입력이 들어오면 종료 코드 0으로 프로그램을 종료합니다.
process.stdin.on('data', process.exit.bind(process, 0))
