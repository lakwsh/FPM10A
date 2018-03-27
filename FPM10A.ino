// 接线方式如下
// [L1 GND] [L2 D3] [L3 D2] [L4 5V] [L5-L6 Null]

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);
// Adafruit_Fingerprint finger=Adafruit_Fingerprint(&mySerial,[数据库密码]);
Adafruit_Fingerprint finger=Adafruit_Fingerprint(&mySerial);

void setup(){
  Serial.begin(9600);
  while(!Serial);
  Serial.println();
  Serial.println("开始运行");
  finger.begin(57600);
  if(finger.verifyPassword()){
    Serial.println("已检测到指纹识别器\n");
  }else{
    Serial.println("没有发现指纹识别器,请检查接线是否正确,或是否正确填写数据库密码");
    while(1) delay(1);
  }
}
int readnumber(void){ // 读取控制指令
  while(!Serial.available());
  return Serial.parseInt();
}
void loop(){
  Serial.println("功能列表:");
  Serial.println("1.添加指纹");
  Serial.println("2.识别模式");
  Serial.println("3.清空指纹库");
  Serial.println("4.删除指纹");
  Serial.println("5.修改数据库密码");
  Serial.print("请选择功能(输入数字): ");
  int id=readnumber();
  Serial.println(id);
  Serial.println();
  switch(id){
    case 1:
      Serial.print("保存指纹,请选择指纹ID(1-127): ");
      id=readnumber();
      Serial.println(id);
      Serial.println();
      if(id>0 and id<128){
        Serial.println("请输入指纹\n");
        if(!getFinger(1)) break;
        Serial.println("请移走手指");
        while(finger.getImage()!=FINGERPRINT_NOFINGER);
        Serial.println("再次输入指纹\n");
        if(!getFinger(2)) break;
        saveFinger(id);
      }else{
        Serial.println("请输入正确的数字!!!");
      }
      break;
    case 2:
      finger.getTemplateCount();
      Serial.print("数据库内有效指纹数目: ");
      Serial.println(finger.templateCount);
      Serial.println();
      Serial.println("请输入指纹");
      getFinger(0);
      searchFinger();
      break;
    case 3:
      Serial.print("清空指纹数据库,请输入'1'确认操作: ");
      id=readnumber();
      Serial.println(id);
      Serial.println();
      if(id==1){
        finger.emptyDatabase();
        Serial.println("指纹数据库已清空");
      }
      break;
    case 4:
      Serial.print("删除指纹,请选择指纹ID(1-127): ");
      id=readnumber();
      Serial.println(id);
      Serial.println();
      if(id>0 and id<128) deleteFinger(id);
      else Serial.println("请输入正确的数字!!!");
      break;
    case 5:
      Serial.print("修改数据库密码,请输入新密码: ");
      id=readnumber();
      Serial.println(id);
      Serial.println();
      if(finger.setPassword(id)==FINGERPRINT_OK) Serial.println("密码已修改");
      else Serial.println("密码修改失败");
      break;
    default:
      Serial.println("请输入正确的数字!!!");
      break;
  }
  Serial.println("\n");
}
bool getFinger(int save){
  int p=-1;
  while(p!=FINGERPRINT_OK){
    // 获取指纹
    p=finger.getImage();
    switch(p){
      case FINGERPRINT_OK:
        Serial.println("成功获取指纹");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("通讯错误");
        return false;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("影像错误");
        return false;
      default:
        Serial.println("未知错误");
        return false;
    }
  }
  // 转换并暂存指纹
  if(save<1 or save>2) p=finger.image2Tz();
  else p=finger.image2Tz(save);
  switch(p){
    case FINGERPRINT_OK:
      Serial.println();
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("指纹过于模糊");
      return false;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通讯错误");
      return false;
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
    default:
      Serial.println("未知错误");
      return false;
  }
  return true;
}
bool saveFinger(int id){
  switch(finger.createModel()){
    case FINGERPRINT_OK:
      Serial.println("已创建指纹模型");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通信错误");
      return false;
    case FINGERPRINT_ENROLLMISMATCH:
      Serial.println("指纹不吻合");
      return false;
    default:
      Serial.println("未知错误");
      return false;
  }
  Serial.println("正在储存指纹");
  switch(finger.storeModel(id)){
    case FINGERPRINT_OK:
      Serial.print("已储存指纹,ID: ");
      Serial.println(id);
      return true;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通信错误");
      break;
    case FINGERPRINT_BADLOCATION:
      Serial.println("错误的ID");
      break;
    case FINGERPRINT_FLASHERR:
      Serial.println("写入错误");
      break;
    default:
      Serial.println("未知错误");
      break;
  }
  return false;
}
bool deleteFinger(int id){
  if(id<1 or id>127) return false;
  switch(finger.deleteModel(id)){
    case FINGERPRINT_OK:
      Serial.print("已删除指纹,ID: ");
      Serial.println(id);
      return true;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通信错误");
      break;
    case FINGERPRINT_BADLOCATION:
      Serial.println("错误的ID");
      break;
    case FINGERPRINT_FLASHERR:
      Serial.println("写入错误");
      break;
    default:
    Serial.print("未知错误");
      break;
  }
  return false;
}
bool searchFinger(){
  switch(finger.fingerFastSearch()){
    case FINGERPRINT_OK:
      Serial.print("匹配到指纹ID: ");
      Serial.print(finger.fingerID);
      Serial.print(",可信度: ");
      Serial.println(finger.confidence);
      return true;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通信错误");
      break;
    case FINGERPRINT_NOTFOUND:
      Serial.println("无匹配的指纹");
      break;
    default:
      Serial.println("未知错误");
      break;
  }
  return false;
}
