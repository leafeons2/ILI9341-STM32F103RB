# ILI9341-STM32F103RB

#### 핵심요약 키워드
>STM32, C, SPI, UART <br />
STM32F103RB, Nucleo-F103RB, W25Q64, ILI9341 <br />
부품조합, 최소 핀 사용, 디버깅

#### 프로젝트 개요
개발 기간 :   2026.06.17 ~ 2026.11  (6달) <br />
목표 : IDLE 상태에서 ILI9341에 움직이는 사진을 출력하고, 
       터치할 때에는 다른 움직이는 사진을 ILI9341에 출력한다. <br />
사용 MCU/보드 : STM32F103RB / Nucleo-F103RB <br /> <br />

환경 : STM32MX 에서 생성된 HAL 함수들을 이용해 STM32IDE 에서 개발 <br />
    : ILI9341 제작사에서 기본으로 제공해주는 인터페이스, 드라이버, 예제 코드  사용 <br />
    : LibDriver 에서 제공하는 W25QXX의 인터페이스, 드라이버 사용
    
### 결과물
구동 영상 : https://youtu.be/cVpCpbx06ak

### 프로젝트의 연결 구상도
<img width="602" height="339" alt="image" src="https://github.com/user-attachments/assets/c9fe0c61-44ca-4768-81ee-75f09423c134" />
