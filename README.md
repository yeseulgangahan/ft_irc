# ft_irc
동시에 다수의 IRC 클라이언트를 처리하는 멀티플렉싱 서버입니다. 문자 기반의 실시간 인터넷 채팅 프로토콜 IRC를 사용하는 공식 IRC 서버의 주요 기능을 구현했습니다.

## Features
- IRC 클라이언트인 IRSSI에 최적화
- 인증 프로세스
- nickname, username 설정
- 채널 생성, 가입, 실시간 채팅, 멤버 관리, 모드 설정
- private message 송수신

## Usage
`make && ./ircserv <port> <password>`
- port: irc서버가 대기할 포트 번호입니다.
- password: irc클라이언트가 연결 시도 시 요구할 비밀번호입니다.

### IRSSI로 서버에 연결하기
```
irssi
/connect 127.0.0.1 <port> <password>
```

### Netcat으로 서버에 연결하기
```
PASS <password>
USER <username> 0 * <realname>
NICK <nickname>
```

## Commands
### 연결
- CAP
- PASS
- NICK
- USER
- PING
- QUIT
### 채널 조작
- JOIN
- PART
- TOPIC
- INVITE
- KICK
### 서버 요청 및 명령
- MODE
### 전송
- PRIVMSG
- NOTICE
### 유저 기반 요청
- WHO

## Peer Reviews
<img width="533" alt="image" src="https://github.com/yeseulgangahan/ft_irc/assets/88709878/ac6c2600-f775-447a-84f2-f588a765e9e5">
<img width="533" alt="image" src="https://github.com/yeseulgangahan/ft_irc/assets/88709878/61b6ad75-8407-47aa-90ea-5d8de44a7c14">
<img width="533" alt="image" src="https://github.com/yeseulgangahan/ft_irc/assets/88709878/1cb2482b-e658-4324-b185-e4f6c360f52b">

## Notion Post
프로젝트를 진행하면서 공부한 내용과 질문을 [작은 IRC 서버를 위한 개념들](https://probable-perch-892.notion.site/IRC-f62548fd0d374caf85fe1e6103c4d980?pvs=4)에 정리했습니다.
