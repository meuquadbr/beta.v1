void iniMPU(){
    Wire.begin();
    mpu.initialize();
    // TWBR = 12; //Defini clock I2C 400kHz
    devStatus = mpu.dmpInitialize();
    
    if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        //Filtro Passa-Baixa
        //MPU6050_DLPF_BW_256         0x00
        //MPU6050_DLPF_BW_188         0x01
        //MPU6050_DLPF_BW_98          0x02
        //MPU6050_DLPF_BW_42          0x03
        //MPU6050_DLPF_BW_20          0x04
        //MPU6050_DLPF_BW_10          0x05
        //MPU6050_DLPF_BW_5           0x06
        mpu.setDLPFMode(0x03); 
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    }
}
void pegaXYZ(){
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    }
    
    angleX = ypr[2] * 180/M_PI;
    angleY = ypr[1] * 180/M_PI;
    angleZ = ypr[0] * 180/M_PI;
}
