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

### Connect to the server with IRSSI
```
irssi
/connect 127.0.0.1 <port> <password>
```

### Connect to the server with a Netcat
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
코드에 대한 자세한 설명은 [ft_irc에 대한 노트](https://probable-perch-892.notion.site/ft_irc-3820bd2af9eb4d56b73439ae3b4bb09b?pvs=4)에 있습니다.
