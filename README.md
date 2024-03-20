## 채팅 서버 프로그램

- **프로젝트 이름** : 게임에서 사용되는 실시간 채팅 서버

- **개발 기간** : 03.06 ~ 03.20

## 설치 및 사용 방법
- **install program**
`git clone https://github.com/dkglee/chatting_service.git`
  
- **start server program** : 루프백 주소(`127.0.0.1`)로 `8080` 포트로 서버가 열리게 된다.
`./chatting_service`
  
- **클라이언트로 서버 연결**
`nc 127.0.0.1 8080`

- **기능 1** : 처음 연결 시 `username`을 입력해야함, 처음 연결되면 general 채널에 가입된다.
- **기능 2** : `/channel [CHANNEL NAME]` 해당 채널에 입장하게 됨. 아무도 없을 시 새로운 채널이 생성됨
- **기능 3** : `/quit` 채널에서 나오게 됨
- **기능 4** : `/msg [TARGET_USERNAME] [TEXT to MESSAGE] 개인 메세지를 보낼 수 있음
- **기능 5** : `[TEXT]` 같은 채널에 있는 사람들에게 메세지가 보내짐

## 개발 배경 및 목적
- 
