# Socket Programming
Learn simple socket  

Environment：Linux

Number|Command|Description
---|:--:|---:
1 | ```/help``` | ```/IMG ALL```<br>```/GAME ALL```<br>```/FILE [filemame]```
2 | ```/IMG ALL``` | Show all emoji command
3 | ```/GAME ALL``` | Show all game command
4 | ```/FILE [fileName]``` | Upload file to server

+ ### ```/IMG ALL```

Number|Command|Description
---|:--:|---:
1 | ```/IMG 1``` | Show emoji ```(っ・Д・)っ```
2 | ```/IMG 2``` | Show emoji ```(･ω´･ )```
3 | ```/IMG 3``` | Show emoji ```(`・ω・´)```
4 | ```/IMG 4``` | Show emoji ```,,Ծ‸Ծ,,```
5 | ```/IMG 5``` | Show emoji ```(╬ﾟдﾟ)```
6 | ```/IMG 6``` | Show emoji ```_(┐「ε:)_```
7 | ```/IMG 7``` | Show emoji ```(*´∀`)~♥```
8 | ```/IMG 8``` | Show emoji ```(๑•́ ₃ •̀๑)```
9 | ```/IMG 9``` | Show emoji ```(ㆆᴗㆆ)```
10 | ```/IMG 10``` | Show emoji ```(◞‸◟)```

+ ### ```/GAME ALL```

Number|Command|Description
---|:--:|---:
1 | ```/GAMEG``` | Play guess the number with the computer
2 | ```/GAMEM``` | Play finger-guessing with the computer

<hr>

# How to use
### 1. Connect
```c
serv_addr.sin_addr.s_addr = inet_addr("10.0.0.1");
```
Multiple connect： ```10.0.0.1```  
Local connect： ```10.0.0.1``` change to ```127.0.0.1```
  
### 2. Compile
Open terminal,enter command：
```c
gcc socketC.c -o socketC
gcc socketS.c -o socketS
```
or
```c
./ MakeFile
```
  
### 3. Run
First,strat server：
```c
./ socketS
```
Next,run client：
```c
./ socketC
```
  
### 4. Command
Enter your command

<hr>

I am a program newbie,first time learn socket,study hard every day!
