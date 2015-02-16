/*#######################################################################
                                 MOTORES
#######################################################################*/

void iniMotores(){
  motor[0].attach(M0_PIN);
  motor[1].attach(M1_PIN);
  motor[2].attach(M2_PIN);
  motor[3].attach(M3_PIN);

  motor[0].writeMicroseconds(MIN_ESC);
  motor[1].writeMicroseconds(MIN_ESC);
  motor[2].writeMicroseconds(MIN_ESC);
  motor[3].writeMicroseconds(MIN_ESC);

  V_MEDIA = 1100;
}

/*#######################################################################
						                      FIM
#######################################################################*/
