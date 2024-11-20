#define BNO_ADDR 0x28

static void init_BNO(){
  myBNO.dev_addr = BNO_ADDR;
  BNO_Init(&myBNO);
  bno055_set_operation_mode(OPERATION_MODE_NDOF);
  delay(10);

  bno055_read_accel_offset_x_axis(&off_ax);
  bno055_read_accel_offset_y_axis(&off_ay);
  bno055_read_accel_offset_z_axis(&off_az);
}

unsigned long lastTime_BNO = 0;

static void read_BNO(){
  if ((millis() - lastTime_BNO) >= 100) //To stream at 10Hz without using additional timers
  {
    lastTime_BNO = millis();

    bno055_read_accel_xyz(&acc_BNO);
    bno055_read_gyro_xyz(&gyro_BNO);
    bno055_read_linear_accel_xyz(&lacc_BNO);

    // float res =  sqrt((acc_BNO.x-off_ax)*(acc_BNO.x-off_ax)+
    //                   (acc_BNO.y-off_ay)*(acc_BNO.y-off_ay)+
    //                   (acc_BNO.z-off_az)*(acc_BNO.z-off_az));

    g_x = (gyro_BNO.x)*0.017;     
    g_y = (gyro_BNO.y)*0.017;         
    g_z = (gyro_BNO.z)*0.017;     

    a_x = ((acc_BNO.x)*9.81)/1000;
    a_y = ((acc_BNO.y)*9.81)/1000;
    a_z = ((acc_BNO.z)*9.81)/1000;
  }
}
