# Air_mouse_pjt
Renesas 보드와 MPU6050의 자이로센서를 이용해, 펌웨어로 제작한 Air Mouse 프로젝트입니다.
![image](https://github.com/woodong11/Air_mouse_pjt/assets/91379630/44142da7-f4d4-4883-9d1f-cf29327fb66a)
[![Video Label](http://img.youtube.com/vi/wf0AXh3au8c/0.jpg)](https://youtu.be/wf0AXh3au8c)

## Project Goal
MPU 6050의 자이로 센서를 이용해 빵판을 기울였을 때 변경되는 가속도 값을 I2C 통신으로 MCU보드에 전송합니다. <br>
이후, MCU에서 PC와 UART 통신을 이용해 데이터를 전송합니다. <br>
visual studio에서 Windows API를 이용해, 들어오는 데이터에 따라 마우스를 제어합니다. <br>

[Renesas 보드 Quick Start(Board Layout) link](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/ra-cortex-m-mcus/fpb-ra6e1-fast-prototyping-board-ra6e1-mcu-group#documents)
: Renesas 회원가입 필요

## circuit connect
<h4>FT232 연결 </h4>
GND ↔ GND, P100 ↔ TXD, P101 ↔ RXD <br>

![image](https://github.com/woodong11/Air_mouse_pjt/assets/91379630/667be085-66a7-472a-ad29-173c1dde8bd1)

<h4>MPU 6050 연결 </h4>
VCC, GND: 빵판의 5v, GND에 연결 <br>
SCL : P400과 연결 + VCC(5v)와 10k옴 저항으로 연결 <br>
SDA : P401과 연결 + VCC(5v)와 10k옴 저항으로 연결 <br>

<h4> 스위치 연결 </h4>
스위치 3개 각각 빵판의 GND에 연결 + VCC(5v)와 10k옴 저항으로 연결(pull up 저항 구성) <br>
각각 P302, P105, P409에 연결 <br>



## e2 studio circuit configuration
require HW: FPB-RA6E1, FT23BL, MPU 6050 <br>
requre SW: Visual Studio, e2 studio <br>

<h4> Pins </h4>
P400, P401 Disabled 모드로 변경, connectivity → IIC0 operation mode : Enabled, P400, P401Output Type : n-ch open drain  <br>
P302, P105, P409를 usr_SW1, usr_SW2, usr_SW3로 이름 변경 <br>
SCI0설정변경 - Asynchronouse UART <br>

<h4> stack </h4>
1. New Stack → Connectivity → UART (r_sci_uart): name - g_uart0, channel - 0, baud rate - 115200,  <br>
callback -user_uart_callback으로 설정<br>
2. New Stack → Connectivity → I2C Master (r_iic_master): slave Addres - 0x68, callback - iic_callback<br>
3. New Stack → input → External IRQ: handler name - g_external_irq5, channel - 5, triger - falling, callback - irq_callback<br>
4. New Stack → input → External IRQ: handler name - g_external_irq0, channel - 0, triger - falling, callback - irq_callback<br>
5. New Stack → input → External IRQ: handler name - g_external_irq6, channel - 6, triger - falling, callback - irq_callback<br>

<h4> BSP </h4>
heap size: 0x1000

<h4> generate </h4>
configuration 이후 code generation으로 생성되면,  <br>
기존의 hal_entry.c 대신 깃허브의 hal_entry.c로 복사 붙여넣기, src에 uart.c 추가하기


## How to run
<h4> prepare on visual studio </h4>
1. open Visual studio2022, make new C++ project (새 프로젝트 만들기 → 빈 프로젝트 →  만들기) <br>
2. add source file : user.cpp (보기 → 솔루션 탐색기 → 소스파일 우클릭 → 추가 → 새항목 → user.cpp 추가 후 내용 붙여넣기)<br>
3. add header file : user.cpp (보기 → 솔루션 탐색기 → 헤더파일 우클릭 → 추가 → 새항목 → serial.h 추가 후 내용 붙여넣기)<br>
4. Change project properties: File → Project 속성 → C/C++ → SDL 검사 : 아니요 적용<br>

<h4> check port number </h4>
1. 윈도우 키보드 버튼 클릭 → 장치 관리자 → Serial port 번호 체크 (for me, COMP10) <br>
2. change port number on user.cpp <br>
3. run MCU on e2 studio (debug 버튼 클릭 → 녹색 실행 버튼 두 번 클릭) <br>
4. run user.cpp (visual studio에서 ctrl + F5) <br>
