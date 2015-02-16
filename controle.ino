/*#######################################################################     
                        CONTROLE DE ESTABILIZAÇÂO
#######################################################################*/

void iniControle(){
  ControleX.SetMode(AUTOMATIC);
  ControleX.SetSampleTime(10);
  ControleX.SetOutputLimits(-MAX_INFLUENCIA_PID,MAX_INFLUENCIA_PID);
  SetpointX = 0;  

  ControleY.SetMode(AUTOMATIC);
  ControleY.SetSampleTime(10);
  ControleY.SetOutputLimits(-MAX_INFLUENCIA_PID,MAX_INFLUENCIA_PID);
  SetpointY = 0; 
  
  ControleZ.SetMode(AUTOMATIC);
  ControleZ.SetSampleTime(5);
  ControleZ.SetOutputLimits(-MAX_INFLUENCIA_PID_Z,MAX_INFLUENCIA_PID_Z);
  SetpointZ = 0; 
}

void controlar(){
  ControleX.Compute();
  ControleY.Compute();
  ControleZ.Compute();

  v[0] = V_MEDIA - OutputX - OutputY - OutputZ;
  v[1] = V_MEDIA + OutputX - OutputY + OutputZ;
  v[2] = V_MEDIA + OutputX + OutputY - OutputZ;
  v[3] = V_MEDIA - OutputX + OutputY + OutputZ;

  v[0] = constrain(v[0], 1000, 2000);
  v[1] = constrain(v[1], 1000, 2000);
  v[2] = constrain(v[2], 1000, 2000);
  v[3] = constrain(v[3], 1000, 2000);

  motor[0].writeMicroseconds(v[0]);
  motor[1].writeMicroseconds(v[1]);
  motor[2].writeMicroseconds(v[2]);
  motor[3].writeMicroseconds(v[3]);
}

/*#######################################################################
                                  FIM
#######################################################################*/
