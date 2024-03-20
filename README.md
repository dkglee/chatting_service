## 채팅 서버 프로그램

- **프로젝트 이름** : 게임에서 사용되는 실시간 채팅 서버

- **개발 기간** : 03.06 ~ 03.20

## 설치 및 사용 방법
- **install program**
  
`git clone https://github.com/dkglee/chatting_service.git`
  
- **start server program** : 루프백 주소(`127.0.0.1`)로 `8080` 포트로 서버가 열리게 된다.

`./chatting_service`
  
- **클라이언트 프로그램 `nc` 사용법**

`nc 127.0.0.1 8080`

- **기능 1** : 처음 연결 시 `username`을 입력해야함, 처음 연결되면 general 채널에 가입된다.
- **기능 2** : `/channel [CHANNEL NAME]` 해당 채널에 입장하게 됨. 아무도 없을 시 새로운 채널이 생성됨
- **기능 3** : `/quit` 채널에서 나오게 됨
- **기능 4** : `/msg [TARGET_USERNAME] [TEXT to MESSAGE]` 개인 메세지를 보낼 수 있음
- **기능 5** : `[TEXT]` 같은 채널에 있는 사람들에게 메세지가 보내짐

## 개발 배경 및 목적
### 상황 가정
- 게임에서 사용되는 채팅 서비스
- 1:1 채팅과 최대 100명의 소규모 그룹 채팅을 지원해야함
- 모든 채팅은 기록되어야 함. (그룹 채팅의 경우 인게임 내의 기록을 다시 보기 위함이 있음)
- 일별 능동 사용자 수(DAU: Daily Active User) 기준으로 2천만명을 처리할 수 있어야 함.
- 텍스트 메시지만 주고 받을수 있음
- 사용자 접속 상태를 표시해야 함
- 알림 기능이 적용되어야 한다.

### 해결해야 할 문제 정의
1. 실시간성을 보장해야함
2. 대화의 내용이 기록되어야 한다.
3. DAU가 2천만명을 수용할 수 있어야 한다.
4. 알림 기능이 적용되어야 한다.

### 지금껏 해결한 문제들의 해결 방법

#### 실시간성과 다수의 요청을 해결하기 위한 방안
1. `epoll`를 이용하여 클라이언트의 입력을 비동기적으로 받을 수 있고 `aio`를 사용하여 비동기 출력을 할 수 있음.
2. 멀티 스레드를 이용하여 클라이언트의 입력을 동시적으로 다수의 스레드가 받을 수 있음. (Boss Thread의 역할을 함)
3. 또한, `worker thread`를 따로 두어 클라이언트의 입력을 작업 큐에 넣어 입력에 대한 결과를 비동기적으로 처리할 수 있음. 또한, `worker thread`를 여럿 두어 처리량을 대폭 늘릴 수 있음.

## 기술 스택 및 도구
- **백엔드** : 서버 측에서 사용된 기술 (C++)
- **개발 환경** : Linux

## 도전 과제

### 멀티 스레드를 다루면서 생긴 문제
[다수의 스레드가 epoll_wait을 했을때 생긴 문제점](https://season-bee-387.notion.site/epoll_wait-e933284e162e46cf9ac8be22b39e6942)

[epoll과 멀티 스레드](https://season-bee-387.notion.site/Multithread-5d44cdcc047e497c87b3e8785f8c963a)

[worker thread 구현하면서 생겼던 문제](https://season-bee-387.notion.site/Boss-Worker-Model-f2f08672ef9a441596086adf19122a4d)

## 배운 점 및 향후 계획
### 배운 점
- 프로젝트를 진행함으로써 실시간성을 극대화하기 위해서 **I/O 요청**을 **비동기**로 처리하는 과정에 대해서 상세하게 배운 것 같다.
- 또한, 처리량을 극대화 하기 위하여 `multi thread`를 사용했는데 이를 사용함으로써 나타난 **"동기화"** 및 **"race"** 문제 등을 해결하는 법에 대해서 기존의 CS 지식을 잘 버무릴 수 있어서 좋았다.
- 리눅스 시스템 네트워크 프로그래밍에 대해서 자세하게 다룬 것 같았다.

### 향후 계획
- 서버에 "on-memory database"인 `redis`를 도입함으로써 서버의 상태를 `redis`로 관리하고 `mongoDB`를 연결하여 특정 시간마다 채팅 내역을 기록하는 시스템을 두어야 할 것 같다.
- `redis`가 일종의 캐시 역할을 할 것으로 기대하며 `scale out`이 쉬운 서버를 만드는 것이 목표다.
- 대규모 사용자의 트래픽을 받아드릴 수 있도록 분산 서버 시스템을 구축해야 할 것 같고 이에 따른 서버끼리의 통신을 `RPC`혹은 `메세지 큐`를 통해 구현할 수 있기를 기대하고 있다.
- 알림 서버와의 연동을 기대하고 있다.
- 최적화
  - 멀티 스레딩에 따른 캐시 일관성에 대한 문제 고민
  - 스케줄링 방법에 대해서 좀 더 고민이 필요함

## Notion Page
- [노션 페이지로 이동](https://season-bee-387.notion.site/Chat-Service-35708aad3f054c059e4f489d0af2c6fc)

