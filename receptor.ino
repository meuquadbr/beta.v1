/*#######################################################################
                            RECEPTOR
#######################################################################*/
// DADOS QUE CHEGAM DO CONTROLE
 // dados[0] = ps2x.Button(PSB_L1);
 //  dados[1] = ps2x.Button(PSB_R1);
 //  dados[2] = ps2x.Button(PSB_L2);
 //  dados[3] = ps2x.Button(PSB_R2);
 //  dados[4] = ps2x.ButtonPressed(PSB_TRIANGLE);
 //  dados[5] = ps2x.ButtonPressed(PSB_CIRCLE);
 //  dados[6] = ps2x.ButtonPressed(PSB_CROSS);
 //  dados[7] = ps2x.ButtonPressed(PSB_SQUARE);  
 //  dados[8] = ps2x.ButtonPressed(PSB_START);
 //  dados[9] = ps2x.ButtonPressed(PSB_SELECT);
 //  dados[10] = ps2x.Button(PSB_PAD_UP);
 //  dados[11] = ps2x.Button(PSB_PAD_LEFT);
 //  dados[12] = ps2x.Button(PSB_PAD_RIGHT);
 //  dados[13] = ps2x.Button(PSB_PAD_DOWN);
 //  dados[14] = ps2x.Analog(PSS_LX);
 //  dados[15] = ps2x.Analog(PSS_LY);
 //  dados[16] = ps2x.Analog(PSS_RX);
 //  dados[17] = ps2x.Analog(PSS_RY);
 //  dados[18] = ps2x.Button(PSB_L3);
 //  dados[19] = ps2x.Button(PSB_R3);
 //  dados[20] = trimagem;

float velocidadeQeuQuero = MIN_ESC;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void iniReceptor(){
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(dados);
  Mirf.config();  
}

void receberSemFio(){
    if(Mirf.dataReady()){
      Mirf.getData((byte *) &dados);
      //Fitra os dados dos botões
      for(int i = 0; i<13;i++){
        if(dados[i]>1) dados[i] = 0;
      }
      //Filtro trimagem
      if(dados[20] > 1) dados[20] = 0;

      // Trata os dados    
      if(dados[5]) {
        gravareeprom();
        while(true){
          motor[0].writeMicroseconds(MIN_ESC);
          motor[1].writeMicroseconds(MIN_ESC);
          motor[2].writeMicroseconds(MIN_ESC);
          motor[3].writeMicroseconds(MIN_ESC);
        }
      }
      // Controle de velocidade
      if(dados[1]) velocidadeQeuQuero = velocidadeQeuQuero + 0.6;
      if(dados[3]) velocidadeQeuQuero = velocidadeQeuQuero + 0.1;
      if(dados[0]) velocidadeQeuQuero = velocidadeQeuQuero - 0.3;
      // if(dados[2]) velocidadeQeuQuero = velocidadeQeuQuero - 0.6;
      
      // SetPoints no analogico esquerdo
      float modX = (dados[14] - 132)/10;
      float modY = (dados[15] - 123)/10;
      SetpointX = MeiodoX + modX;
      SetpointY = MeiodoY + modY;
      SetpointX = constrain(SetpointX,MeiodoX-ANGULO_MAXIMO,MeiodoX+ANGULO_MAXIMO);
      SetpointY = constrain(SetpointY,MeiodoY-ANGULO_MAXIMO,MeiodoY+ANGULO_MAXIMO);
      
      //Setpoint Z analogico direito 
      float modZ = (dados[16] - 123)/50;
      SetpointZ = SetpointZ + modZ/30;
            
      // X volta o Z para o ponto original
      if(dados[6]) SetpointZ = CENTROZ;
      
      //Trimagem
      if(dados[20]) {
        if(dados[10]) MeiodoY = MeiodoY - 0.001;
        if(dados[13]) MeiodoY = MeiodoY + 0.001;
        if(dados[12]) MeiodoX = MeiodoX + 0.001;
        if(dados[11]) MeiodoX = MeiodoX - 0.001;
      }

      // O botão L3 aumenta o angulo SP em 10 graus
      if(dados[18]){
        if(SetpointX<MeiodoX) SetpointX = SetpointX - 10;
        if(SetpointX>MeiodoX) SetpointX = SetpointX + 10;
        if(SetpointY<MeiodoY) SetpointY = SetpointY - 10;
        if(SetpointY>MeiodoY) SetpointY = SetpointY + 10;
      }

  }
  V_MEDIA = velocidadeQeuQuero;
}