# FPM10A
Arduino FPM10A fingerprint sensor

## 使用的Libraey
<a href="https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library">Adafruit Fingerprint Sensor Library</a>

## 可能需要的工具
<a href="https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html">PuTTY</a>

## 功能一览
<ol>
  <li>添加指纹</li>
  <li>识别指纹</li>
  <li>清空指纹</li>
  <li>删除指纹</li>
  <li>修改数据库密码</li>
</ol>

## 公开函数表
1.Instantiates sensor with Software/Hardware Serial<br>
  -> `ss/hs` Pointer to HardwareSerial object<br>
  -> `password` 32-bit integer password (default is 0)<br>
	`Adafruit_Fingerprint::Adafruit_Fingerprint(SoftwareSerial *ss, uint32_t password)`<br>
	`Adafruit_Fingerprint::Adafruit_Fingerprint(HardwareSerial *hs, uint32_t password)`<br>
<br>
2.Initializes serial interface and baud rate<br>
  -> `baudrate` Sensor's UART baud rate (usually 57600, 9600 or 115200)<br>
  `void Adafruit_Fingerprint::begin(uint32_t baudrate)`<br>
<br>
3.Verifies the sensors' access password (default password is 0x0000000).<br>
  A good way to also check if the sensors is active and responding<br>
  -> returns `True` if password is correct<br>
  `boolean Adafruit_Fingerprint::verifyPassword(void)`<br>
  `uint8_t Adafruit_Fingerprint::checkPassword(void)`<br>
<br>
4.Ask the sensor to take an image of the finger pressed on surface<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_NOFINGER` if no finger detected<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  -> returns `FINGERPRINT_IMAGEFAIL` on imaging error<br>
  `uint8_t Adafruit_Fingerprint::getImage(void)`<br>
<br>
5.Ask the sensor to convert image to feature template<br>
  -> `slot` Location to place feature template (put one in 1 and another in 2 for verification to create model)<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_IMAGEMESS` if image is too messy<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  -> returns `FINGERPRINT_FEATUREFAIL` on failure to identify fingerprint features<br>
  -> returns `FINGERPRINT_INVALIDIMAGE` on failure to identify fingerprint features<br>
  `uint8_t Adafruit_Fingerprint::image2Tz(uint8_t slot)`<br>
<br>
6.Ask the sensor to take two print feature template and create a model<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  -> returns `FINGERPRINT_ENROLLMISMATCH` on mismatch of fingerprints<br>
  `uint8_t Adafruit_Fingerprint::createModel(void)`<br>
<br>
7.Ask the sensor to store the calculated model for later matching<br>
  -> `location` The model location<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_BADLOCATION` if the location is invalid<br>
  -> returns `FINGERPRINT_FLASHERR` if the model couldn't be written to flash memory<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::storeModel(uint16_t location)`<br>
<br>
8.Ask the sensor to load a fingerprint model from flash into buffer 1<br>
  -> `location` The model location<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_BADLOCATION` if the location is invalid<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::loadModel(uint16_t location)`<br>
<br>
9.Ask the sensor to transfer 256-byte fingerprint template from the buffer to the UART<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::getModel(void)`<br>
<br>
10.Ask the sensor to delete a model in memory<br>
  -> `location` The model location<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_BADLOCATION` if the location is invalid<br>
  -> returns `FINGERPRINT_FLASHERR` if the model couldn't be written to flash memory<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::deleteModel(uint16_t location)`<br>
<br>
11.Ask the sensor to delete ALL models in memory<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_BADLOCATION` if the location is invalid<br>
  -> returns `FINGERPRINT_FLASHERR` if the model couldn't be written to flash memory<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::emptyDatabase(void)`<br>
<br>
12.Ask the sensor to search the current slot 1 fingerprint features to match saved templates.<br>
   The matching location is stored in <b>fingerID</b> and the matching confidence in <b>confidence</b><br>
  -> returns `FINGERPRINT_OK` on fingerprint match success<br>
  -> returns `FINGERPRINT_NOTFOUND` no match made<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::fingerFastSearch(void)`<br>
<br>
13.Ask the sensor for the number of templates stored in memory.<br>
   The number is stored in <b>templateCount</b> on success.<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::getTemplateCount(void)`<br>
<br>
14.Set the password on the sensor<br>
  -> `password` 32-bit password code<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_PACKETRECIEVEERR` on communication error<br>
  `uint8_t Adafruit_Fingerprint::setPassword(uint32_t password)`<br>
<br>
15.Helper function to process a packet and send it over UART to the sensor<br>
  -> `packet` A structure containing the bytes to transmit<br>
  `void Adafruit_Fingerprint::writeStructuredPacket(const Adafruit_Fingerprint_Packet & packet)`<br>
<br>
16.Helper function to receive data over UART from the sensor and process it into a packet<br>
  -> `packet` A structure containing the bytes received<br>
  -> `timeout` how many milliseconds we're willing to wait<br>
  -> returns `FINGERPRINT_OK` on success<br>
  -> returns `FINGERPRINT_TIMEOUT` or `FINGERPRINT_BADPACKET` on failure<br>
  `uint8_t Adafruit_Fingerprint::getStructuredPacket(Adafruit_Fingerprint_Packet * packet, uint16_t timeout)`
