# Linux_Chat_ComputerNetwork2020

### 리눅스 환경에서 Multiplexing을 이용한 채팅 프로그램과 Multithreading을 이용한 채팅 프로그램 구현

### 1_Multiplexing_Chat_App

#### 프로그램 설명
- Multiplexing chat server를 이용하여 여러 client가 접속 후 채팅이 가능함.
- 전송된 메시지는 모든 클라이언트가 볼 수 있음.
- 최대 Message buffer: 100.
- timeout value = sec 5, usec 5000.
- Client based on TCP multiprocess with I/O routine split.

#### 사용 방법
- 서버
  + 코드 컴파일
  + "[실행파일 이름] [ip 주소] [port 번호]" 입력
- 클라이언트
  + 코드 컴파일
  + 클라이언트들은 각각 "[실행파일 이름] [IP 주소] [port 번호] [이름]" 입력하여 서버와 연결
  + 전송하고자 하는 메시지 입력
  
#### 예시
![cn hw 1 eg2](https://user-images.githubusercontent.com/55964775/89794085-88b38b00-db61-11ea-95cb-1a9074943c85.jpg){: width="100" height="100"}


***

### 2_Multithreading_Chat_App
#### 프로그램 설명
- Multithreaded chat application
- 특정 client에게 메시지를 전송하거나 전체 client들에게 메시지 전송 가능.
- 클라이언트가 접속하거나 떠날 때 서버 측에서는 이름과 상태 출력.
- 클라이언트가 메시지를 받으면 받은 메시지 내용과 보낸 클라이언트 이름을 출력.
- 메시지를 보낼 때에는 "@[상대방 이름] [메시지 내용]" 입력.
- 상대방 이름 대신 "@all"을 입력하면 클라이언트 전체에게 메시지를 보낼 수 있음.

#### 사용 방법
- 서버
  + 코드 컴파일
  + "[실행파일 이름] [ip 주소] [port 번호]" 입력
- 클라이언트
  + 코드 컴파일
  + 클라이언트들은 각각 "[실행 파일 이름]_[IP 주소] [port 번호] [이름]" 입력하여 서버와 연결
  + "@[수신자 이름] [메시지 내용]" 입력하여 특정 상대에게 메시지 전송
  
#### 예시
![cn hw 2 eg2](https://user-images.githubusercontent.com/55964775/89794316-db8d4280-db61-11ea-9d52-c7a4f1b6c8e8.jpg)

#### 강의명
인하대학교 2020년 1학기 Computer Network
