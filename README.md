Mini project Stopwatch IoT subject

![alt text](https://github.com/MeRrai333/esp8266StopwatchGroup8/blob/main/image/Image.jpg?raw=true)

===== Material =====
1. ESP8266 1 unit
2. LCD i2C 16x2 1 unit 
3. push switch 2 unit
4. led 1 unit
5. resistor 220Ohm 1 unit
6. Relay module 3.3V 1 unit

![alt text](https://github.com/MeRrai333/esp8266StopwatchGroup8/blob/main/image/Schematic.JPG?raw=true)

===== State work ======
TIMERSTOPSTATE<br>
  D5: Start<br>
  D6: Reset/ กดค้าง 2 วิ(LED ติด) เปลี่ยน Mode<br>
TIMERSTATE<br>
  D5: Stop<br>
COUNTDOWNSETSTATE<br>
  D5: Inces.<br>
  D6: Deces. / กดค้าง 2 วิ(LED ติด) เปลี่ยนหลัก เปลี่ยนจนครบจะไป Mode ถัดไป / กดค้าง 4 วิ(LED ดับ) RESET กลับไป TIMER<br>
COUNTDOWNSTOPSTATE<br>
  D5: Start<br>
  D6: Reset ตั้งค่าใหม่/ กดค้าง 2 วิ(LED ติด) Reset กลับไป TIMER/ กดค้าง 4 วิ(LED ดับ) ไป SETRELAYSTATE<br>
COUNTDOWNSTARTSTATE<br>
  D5: Stop<br>
  เวลาหมด: ไป COUNTDOWNFINISHSTATE<br>
COUNTDOWNFINISHSTATE<br>
  D6: Reset ตั้งค่าใหม่/ กดค้าง 2 วิ(LED ติด) Reset กลับไป TIMER<br>
SETRELAYSTATE<br>
  D5: เปลี่ยน Mode (OFF, ON->OFF, OFF->ON)<br>
D6: ไป COUNTDOWNSTOPSTATE<br>
RESET<br>
  reset แล้วกลับไป state หลักเดิม<br>

![alt text](https://github.com/MeRrai333/esp8266StopwatchGroup8/blob/main/image/State.JPG?raw=true)

===== Group 8 damn =====
1. นายณฐกฤษฎิ์ ธรรมธัชติสรณ์ 116530462021-4
2. นายเมธิส สมพิทักษ์ 116530462029-7
