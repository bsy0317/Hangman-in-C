# Hangman-in-C
>C언어로 구현한 Hangman 네트워크 게임

pipe, fork() 를 이용하여 부모>자식간 pipe 통신을 이용하여 서버에서 데이터를 처리합니다.

![](../header.png)

## 설치 방법

C
```sh
sudo apt-get install gcc
gcc server.c -o server
gcc client.c -o client
```

## 실행방법
```sh
./server 5050
./client 127.0.0.1 5050
```
서버에 별 다른 인자값을 주지 않고 실행 할 경우 5050번 포트를 디폴트로 사용합니다.

## 업데이트 내역

* 0.1.0
    * 첫 출시
* 0.0.1
    * 작업 진행 중

## 정보

배서연 – talk@kakao.one

GPL 3.0 라이센스를 준수하며 ``LICENSE``에서 자세한 정보를 확인할 수 있습니다.
[https://opensource.org/licenses/gpl-3.0.html]

## 기여 방법

1. (<https://github.com/bsy0317/Hangman-in-C/fork>)을 포크합니다.
2. (`git checkout -b feature/fooBar`) 명령어로 새 브랜치를 만드세요.
3. (`git commit -am 'Add some fooBar'`) 명령어로 커밋하세요.
4. (`git push origin feature/fooBar`) 명령어로 브랜치에 푸시하세요. 
5. 풀리퀘스트를 보내주세요.
