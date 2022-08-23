//############################################################################################################################################################PH_sol_nut############################################ PRE SETUP
//---------------------------------------------------------------------------------------------------- LIBRERIAS
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <OneWire.h>
#include <DS18B20.h>
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- VARIABLES AUXILIARES
int evento_inicio_sistema = 0;

#define dia 0
#define noche 1

#define pwm_OFF 500
#define OFF 0
#define ON 1

#define MIN 0
#define MAX 1
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION DE SISTEMA Y ETAPAS DE CULTIVO
struct parametros_cultivo // ESTRUCTURA -> PARAMETROS WEEK'S DE CULTIVO IDEAL
            {
             int fotoperiodo;
             String etapa_cultivo;             
             int temp_sol_nut;
             int temp_amb[2];
             int hum_amb[2];
             int ph_sol_nut[2];
             int ppm_sol_nut[2];
             int recirculacion_sol_nut[2];
             int recambio_aire[2];
            };

String comando_recivido_desde_pc_1 = "";
String comando_recivido_desde_pc_2 = "";
unsigned long aux_comando_recivido_desde_pc_2 = 0;
unsigned long aux_control_inicio_cultivo = 3600000;

int CULTIVO_INICIADO = OFF;
int CONFIRMACION_INICIO_CULTIVO = OFF;
int CONFIRMACION_AVANCE_ETAPA_DE_CULTIVO = OFF;

int horas_cultivo_total = 0;
int dias_cultivo_total = 0;
int valor_anterior_dias_cultivo_total = 0;

int dias_cultivo_GERMINACION = 0;
int dias_cultivo_PLANTULA = 0;
int dias_cultivo_VEGETACION = 0;
int dias_cultivo_FLORACION = 0;
int dias_cultivo_COSECHA = 0;
int dias_cultivo_MANICURA_mas_REVEGETACION = 0;
int dias_cultivo_SECADO_mas_REVEGETACION = 0;
int dias_cultivo_CURADO_mas_REVEGETACION = 0;
int dias_cultivo_RE_FLORACION = 0;
int dias_cultivo_RE_COSECHA = 0;
int dias_cultivo_RE_MANICURA = 0;
int dias_cultivo_RE_SECADO = 0;
int dias_cultivo_RE_CURADO = 0;

int week_cultivo_actual_ideal = 0;
int contador_dias_week_cultivo_actual_ideal = 0;

int fotoperiodo_optimo_ON = -100;
int fotoperiodo_optimo_OFF = -100;
int temp_sol_nut_optimo = -100;
int temp_amb_dia_optimo = -100;
int temp_amb_noche_optimo = -100;
int hum_amb_dia_optimo = -100;
int hum_amb_noche_optimo = -100;
int PH_sol_nut_optimo_MIN = -100;
int PH_sol_nut_optimo_MAX = -100;
float PPM_sol_nut_optimo_MIN = -100.0;
float PPM_sol_nut_optimo_MAX = -100.0;
int recirculacion_sol_nut_ON_optimo = -100;
int recirculacion_sol_nut_OFF_optimo = -100;
int recambio_aire_ON_optimo = -100;
int recambio_aire_OFF_optimo = -100;

String PROTECTOR_DESBALANCE_SOL_NUT = "";
int fotoperiodo_actual = -1;

int aux_ciclo_peltier_1 = ON;
int aux_ciclo_peltier_2 = OFF;
int aux_ciclo_peltier_3 = OFF;

int aux_ciclo_recirculacion_sol_nut_1 = ON;
int aux_ciclo_recirculacion_sol_nut_2 = OFF;
int aux_ciclo_recirculacion_sol_nut_3 = OFF;

int aux_ciclo_recambio_aire_1 = ON;
int aux_ciclo_recambio_aire_2 = OFF;
int aux_ciclo_recambio_aire_3 = OFF;

int aux_ciclo_periodo_luz_1 = ON;
int aux_ciclo_periodo_luz_2 = OFF;
int aux_ciclo_periodo_luz_3 = OFF;

int aux_stop_compuerta_aire = OFF;

float V_EC_mas = 0.0;
float V_EC_menos = 0.0;
int int_V_EC_mas = 0;
int int_V_EC_menos = 0;

String millis_V_EC_mas = "";
String millis_V_EC_menos = "";

int aux_activar_control_PH = OFF;

String codigo_leido_desde_consola = "";

int aux_calibracion = 0;
//------------------------------------------------------ CALIBRACION CONTROLES LOGICOS PARA MANTENER PUNTO OPTIMO
float holgura_permitida_alerta_desbalance_EC = 25.0;
int holgura_permitida_alerta_desbalance_PH = 1;
int holgura_permitida_alerta_desbalance_temp_amb = 3;
int holgura_permitida_alerta_desbalance_hum_amb = 5;
int holgura_permitida_alerta_desbalance_temp_sol_nut = 3;

int holgura_permitida_control_temp_amb = 1;
int holgura_permitida_control_hum_amb = 2;
int holgura_permitida_control_temp_sol_nut = 1;

unsigned long tiempo_max_modo_calor_deshumidificador = 5000;
unsigned long tiempo_max_modo_frio_deshumidificador = 1200000;

int hora_inicio_fotoperiodo_dia = 10;

int PPM_equivalente_nada = 10;

float porcentaje_EC_ideal_desde_promedio = 100.0;

float valor_sobre_calibracion_EC = 0.560;   // 1 EC = 1 mS/cm = 1000 uS/cm
float K_PPM_mas = 0.49;
float K_PPM_menos = 0.49;
float K_EC_sol_nut = 1.00;

float valor_sobre_calibracion_PH_ValorMenor6 = 40.1;
float valor_sobre_calibracion_PH_ValorMayor6 = 70.1; 

float volumen_bajo_sensor_1_sol_nut = 12838.0;
float volumen_bajo_sensor_2_sol_nut = 10004.0;

float volumen_ramas_bajo_sensor_2 = 0.0;
float volumen_ramas_entre_sensor_1_y_2 = 0.0;

int millis_dosis_PH_mas = 0;
int millis_dosis_PH_menos = 0;

float calibracion_ph_Y = 222.49;
float calibracion_ph_M = -56.48;

int oooooI_potencia_VENTILADOR_PRINCIPAL = 530;
int ooooII_potencia_VENTILADOR_PRINCIPAL = 535;
int oooIII_potencia_VENTILADOR_PRINCIPAL = 540;
int ooIIII_potencia_VENTILADOR_PRINCIPAL = 545;
int oIIIII_potencia_VENTILADOR_PRINCIPAL = 550;
int IIIIII_potencia_VENTILADOR_PRINCIPAL = 555;
//______________________________________________________
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION DE LOS PERIFERICOS
#define pin_LED 43
#define pin_BOMBA_RECIRCULACION_SOL_NUT 42
#define pin_BOMBA_AIRE_OXIGENACION_SOL_NUT 41
#define pin_BOMBA_LECTURA_PH_EC 40
#define pin_VALVULA_PH_mas 39
#define pin_VALVULA_PH_menos 38
#define pin_TERMOVENTILADOR A8
#define pin_HUMIDIFICADOR 36
#define pin_DISIPADOR_PELTIER_DESHUMIDIFICADOR 34
#define pin_CALEFACTOR_SOL_NUT 33
#define pin_BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT 32
#define pin_VENTILADORES_RECAMBIO_AIRE 31
#define pin_VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT 30
#define pin_PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT 2
#define pin_PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT 3
#define pin_NC_FIX A9

int LED = OFF;
int BOMBA_RECIRCULACION_SOL_NUT = OFF;
int BOMBA_AIRE_OXIGENACION_SOL_NUT = OFF;
int BOMBA_LECTURA_PH_EC = OFF;
int VALVULA_PH_mas = OFF;
int VALVULA_PH_menos = OFF;
int TERMOVENTILADOR = OFF;
int HUMIDIFICADOR = OFF;
int old_pin_36 = -1;
int DISIPADOR_PELTIER_DESHUMIDIFICADOR = OFF;
int CALEFACTOR_SOL_NUT = OFF;
int BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
int VENTILADORES_RECAMBIO_AIRE = OFF;
int VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
int PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
int PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
int FUENTE_12V48A = -1;
int NC_FIX = OFF;
int PELTIER_DESHUMIDIFICADOR = pwm_OFF;
int VENTILADOR_PRINCIPAL = pwm_OFF;
int COMPUERTA_AIRE = pwm_OFF;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION I2C
int I2C_BUSY = OFF;
String mensaje_por_enviar_al_NodeMcu;
const int largo_mensaje_I2C = 8;
char char_mensaje_recibido_del_NodeMcu[largo_mensaje_I2C + 1];
char char_mensaje_por_enviar_al_NodeMcu[largo_mensaje_I2C + 1];
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION RELOJ
int hora_actual = -1;
int hora_anterior = -1;

unsigned long aux_frecuencia_1 = 0;
unsigned long aux_frecuencia_2 = 0;
unsigned long aux_frecuencia_3 = 0;
unsigned long aux_frecuencia_4 = 0;
unsigned long aux_frecuencia_5 = 0;
unsigned long aux_frecuencia_6 = 0;
unsigned long aux_frecuencia_7 = 0;
unsigned long aux_frecuencia_8 = 0;
unsigned long aux_frecuencia_9 = 0;
unsigned long aux_frecuencia_10 = 0;
unsigned long aux_frecuencia_11 = 0;
unsigned long aux_frecuencia_12 = 0;
unsigned long aux_frecuencia_13 = 0;
unsigned long aux_frecuencia_14 = 0;
unsigned long aux_frecuencia_15 = 0;
unsigned long aux_frecuencia_16 = 0;
unsigned long aux_frecuencia_17 = 0;
unsigned long aux_frecuencia_18 = 0;
unsigned long aux_frecuencia_19 = 0;
unsigned long aux_frecuencia_20 = 0;
unsigned long aux_frecuencia_21 = 0;
unsigned long aux_frecuencia_22 = 0;
unsigned long aux_frecuencia_23 = 0;
unsigned long aux_frecuencia_24 = 0;
unsigned long aux_frecuencia_25 = 0;
unsigned long aux_frecuencia_26 = 0;
unsigned long aux_frecuencia_27 = 0;
unsigned long aux_frecuencia_28 = 0;
unsigned long aux_frecuencia_34 = 0;
unsigned long aux_frecuencia_35 = 0;
unsigned long aux_frecuencia_36 = 0;
unsigned long aux_frecuencia_37 = 0;
unsigned long aux_frecuencia_38 = 0;
unsigned long aux_frecuencia_39 = 0;
unsigned long aux_frecuencia_40 = 0;
unsigned long aux_frecuencia_41 = 0;
unsigned long aux_frecuencia_42 = 0;
unsigned long aux_frecuencia_43 = 0;
unsigned long aux_frecuencia_44 = 0;
unsigned long aux_frecuencia_45 = 0;
unsigned long aux_frecuencia_46 = 0;
unsigned long aux_frecuencia_47 = 0;
unsigned long aux_frecuencia_48 = 0;
unsigned long aux_frecuencia_49 = 0;
unsigned long aux_frecuencia_50 = 0;
unsigned long aux_frecuencia_51 = 0;
unsigned long aux_frecuencia_52 = 0;
unsigned long aux_frecuencia_53 = 0;
unsigned long aux_frecuencia_54 = 0;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSORES DHT22 1,2,3,4
int temp_amb_1 = -1;
int temp_amb_2 = -1;
int temp_amb_3 = -1;
int temp_amb_4 = -1;
int promedio_temp_amb = -1;

int hum_amb_1 = -1;
int hum_amb_2 = -1;
int hum_amb_3 = -1;
int hum_amb_4 = -1;
int promedio_hum_amb = -1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSORES DS18B20 1,2
int temp_sol_nut = -1;
int aux_temp_sol_nut = 0;
int aux_1_calibracion_temp_sol_nut = OFF;

int temp_deshumidificador = -1;
int activar_sensor_temp_deshumidificador = -1;

#define pin_SENSOR_TEMP_SOL_NUT 8

OneWire onewire_SENSOR_TEMP_SOL_NUT(pin_SENSOR_TEMP_SOL_NUT);
DS18B20 SENSOR_TEMP_SOL_NUT(&onewire_SENSOR_TEMP_SOL_NUT);
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSORES NIVEL DE AGUA
int sensor_nivel_1_RECP_EC_menos = -1;
float sensor_PPM_nivel_2_RECP_EC_menos = -1.0;
float sensor_EC_nivel_2_RECP_EC_menos = -1.0;

#define pin_5v_sensor_nivel_1_RECP_EC_mas 7
#define pin_sensor_nivel_1_RECP_EC_mas A7
int sensor_nivel_1_RECP_EC_mas = -1;

float sensor_PPM_nivel_2_RECP_EC_mas = -1.0;
float sensor_EC_nivel_2_RECP_EC_mas = -1.0;


#define pin_5v_sensor_nivel_extra_superior 5
#define pin_sensor_nivel_extra_superior A5
int aux_old_sistema_humidificador = -1;

int sensor_nivel_aprox_RECP_HUM = -1;
int aux_sensor_nivel_aprox_RECP_HUM = 0;

int sensor_nivel_1_RECP_SOL_NUT = -1;

#define pin_5v_sensor_nivel_2_RECP_SOL_NUT 4
#define pin_sensor_nivel_2_RECP_SOL_NUT A4
int sensor_nivel_2_RECP_SOL_NUT = -1;

int sensor_nivel_RECP_PH_mas = -1;
int sensor_nivel_RECP_PH_menos = -1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR PPM
float PPM_sol_nut = -1.0;
float EC_sol_nut = -1.0;
int aux_inicio_EC_BOMBA_LECTURA_PH_EC = ON;
unsigned long aux_lectura_EC = 3600000;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR PH
#define pin_sensor_PH A0
float promedio_PH_sol_nut = 0.0;
int buf[10];
int temporal = 0;
int PH_sol_nut = 0;
int aux_1_calibracion_sensor_PH = OFF;
float aux_calibracion_sensor_PHValorMenor6 = 0.0;
float aux_calibracion_sensor_PHValorMayor6 = 0.0;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION MICRO SD
#define SD_CS 53
File MICRO_SD;
int PUNTERO_ARCHIVO_MICRO_SD = 1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION MP3 PLAYER
String PLAYLIST = "999";
String mp3_no_repeat = "965";
String TEMA_CARGANDO = "002";
String TEMA_INICIO_CULTIVO = "003";
String TEMA_FIN_CULTIVO = "004";
String TEMA_LECTURA_PH = "005";
String BORRAR_EPRROM = "987";
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR PUERTA
int puerta_abierta = -1;
int aux_alerta_puerta_abierta = ON;
int aux_puerta_abierta = 5;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION BOTON USER FRIENDLY
#define pin_boton_lectura_ph A1
int aux_estado_anterior_boton_user_friendly = ON;

unsigned long aux_tiempo_presionado_boton_user_friendly = 0;
unsigned long tiempo_presionado_boton_user_friendly = 0;

int boton_RE_FILL_RESERVA_HUMIDIFICADOR_presionado = OFF;
int boton_lectura_ph_presionado = OFF;

int activar_boton_RE_FILL_RESERVA_HUMIDIFICADOR = OFF;
int activar_boton_lectura_ph = OFF;

int aux_actualizacion_por_lectura_PH = OFF;
//____________________________________________________________________________________________________
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## FUNCIONES
//----------------------------------------------------------------------------------------------------
void I2C_s__RECEIVE(int howMany)
								{
								 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																			{
																			 char_mensaje_recibido_del_NodeMcu[i] =' ';
																			}
								 int aux = 0;
								 while(Wire.available())
													  {
													   char_mensaje_recibido_del_NodeMcu[aux]= Wire.read();
													   aux = aux + 1;
													  }
								}
//____________________________________________________________________________________________________
//----------------------------------------------------------------------------------------------------
void I2C_s__SEND()
					{
					 Wire.write(mensaje_por_enviar_al_NodeMcu.c_str());
					}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C PERIFERICOS
void I2C_perifericos(String periferico_abrebiado, String opcion)
						{
						 mensaje_por_enviar_al_NodeMcu = String(String(periferico_abrebiado) + String(",") + String(opcion));
						 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
						 I2C_BUSY = ON;
						 while(I2C_BUSY == ON)
											 {
											  Wire.onRequest(I2C_s__SEND);
											  Wire.onReceive(I2C_s__RECEIVE);

											  if(char_mensaje_recibido_del_NodeMcu[0] == char_mensaje_por_enviar_al_NodeMcu[0] &&
												 char_mensaje_recibido_del_NodeMcu[1] == char_mensaje_por_enviar_al_NodeMcu[1] &&
												 char_mensaje_recibido_del_NodeMcu[2] == char_mensaje_por_enviar_al_NodeMcu[2] &&
												 char_mensaje_recibido_del_NodeMcu[3] == char_mensaje_por_enviar_al_NodeMcu[3] &&
												 char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4] &&
												 char_mensaje_recibido_del_NodeMcu[5] == char_mensaje_por_enviar_al_NodeMcu[5] &&
												 char_mensaje_recibido_del_NodeMcu[6] == char_mensaje_por_enviar_al_NodeMcu[6] &&
												 char_mensaje_recibido_del_NodeMcu[7] == char_mensaje_por_enviar_al_NodeMcu[7])
																															 {
																															  I2C_BUSY = OFF;
																															  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																										{
																																										 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																										}

																															  digitalWrite(A15, HIGH);
																															  delay(100);
																															  digitalWrite(A15, LOW);
																															  delay(100);
																															 }
											 }
						}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C TOCAR CANCION
void I2C_tocar_cancion(String track)
						{
						 mensaje_por_enviar_al_NodeMcu = String(String("play") + String(",") + String(track));
						 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
						 I2C_BUSY = ON;
						 while(I2C_BUSY == ON)
											 {
											  Wire.onRequest(I2C_s__SEND);
											  Wire.onReceive(I2C_s__RECEIVE);

											  if(char_mensaje_recibido_del_NodeMcu[0] == char_mensaje_por_enviar_al_NodeMcu[0] &&
												 char_mensaje_recibido_del_NodeMcu[1] == char_mensaje_por_enviar_al_NodeMcu[1] &&
												 char_mensaje_recibido_del_NodeMcu[2] == char_mensaje_por_enviar_al_NodeMcu[2] &&
												 char_mensaje_recibido_del_NodeMcu[3] == char_mensaje_por_enviar_al_NodeMcu[3] &&
												 char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4] &&
												 char_mensaje_recibido_del_NodeMcu[5] == char_mensaje_por_enviar_al_NodeMcu[5] &&
												 char_mensaje_recibido_del_NodeMcu[6] == char_mensaje_por_enviar_al_NodeMcu[6] &&
												 char_mensaje_recibido_del_NodeMcu[7] == char_mensaje_por_enviar_al_NodeMcu[7])
																															 {
																															  I2C_BUSY = OFF;
																															  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																										{
																																										 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																										}
																															 }
											 }
						}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C TODO OK
void I2C_TODO_OK()
				{
				 mensaje_por_enviar_al_NodeMcu = String("okgrower");
				 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
				 I2C_BUSY = ON;
				 while(I2C_BUSY == ON)
									 {
									  Wire.onRequest(I2C_s__SEND);
									  Wire.onReceive(I2C_s__RECEIVE);

									  if(char_mensaje_recibido_del_NodeMcu[0] == char_mensaje_por_enviar_al_NodeMcu[0] &&
										 char_mensaje_recibido_del_NodeMcu[1] == char_mensaje_por_enviar_al_NodeMcu[1] &&
										 char_mensaje_recibido_del_NodeMcu[2] == char_mensaje_por_enviar_al_NodeMcu[2] &&
										 char_mensaje_recibido_del_NodeMcu[3] == char_mensaje_por_enviar_al_NodeMcu[3] &&
										 char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4] &&
										 char_mensaje_recibido_del_NodeMcu[5] == char_mensaje_por_enviar_al_NodeMcu[5] &&
										 char_mensaje_recibido_del_NodeMcu[6] == char_mensaje_por_enviar_al_NodeMcu[6] &&
										 char_mensaje_recibido_del_NodeMcu[7] == char_mensaje_por_enviar_al_NodeMcu[7])
																													 {
																													  I2C_BUSY = OFF;
																													  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																								{
																																								 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																								}
																													 delay(100);
																													 }
									 }
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C LECTURA SENSOR
void I2C_lectura_sensor(String sensor_abrebiado)
				{
				 mensaje_por_enviar_al_NodeMcu = String(String(sensor_abrebiado) + String(",") + String("LEC"));
				 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
				 I2C_BUSY = ON;
				 while(I2C_BUSY == ON)
									 {
									  Wire.onRequest(I2C_s__SEND);
									  Wire.onReceive(I2C_s__RECEIVE);

									  if(char_mensaje_recibido_del_NodeMcu[0] == char_mensaje_por_enviar_al_NodeMcu[0] &&
										 char_mensaje_recibido_del_NodeMcu[1] == char_mensaje_por_enviar_al_NodeMcu[1] &&
										 char_mensaje_recibido_del_NodeMcu[2] == char_mensaje_por_enviar_al_NodeMcu[2] &&
										 char_mensaje_recibido_del_NodeMcu[3] == char_mensaje_por_enviar_al_NodeMcu[3] &&
										 char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4])
																													 {
																													  I2C_BUSY = OFF;

																													  digitalWrite(13, HIGH);
																													  delay(100);
																													  digitalWrite(13, LOW);
																													  delay(100);
																													 }
									 }
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- GUARDAR BACKUP EN MICRO SD
void SaveData()
			   {
				digitalWrite(A11, HIGH);
				delay(100);

				SD.remove("datos.txt");
				MICRO_SD = SD.open("datos.txt", FILE_WRITE);

				MICRO_SD.println(CULTIVO_INICIADO);
				MICRO_SD.println(CONFIRMACION_INICIO_CULTIVO);
				MICRO_SD.println(CONFIRMACION_AVANCE_ETAPA_DE_CULTIVO);
				MICRO_SD.println(horas_cultivo_total);
				MICRO_SD.println(dias_cultivo_total);
				MICRO_SD.println(valor_anterior_dias_cultivo_total);
				MICRO_SD.println(dias_cultivo_GERMINACION);
				MICRO_SD.println(dias_cultivo_PLANTULA);
				MICRO_SD.println(dias_cultivo_VEGETACION);
				MICRO_SD.println(dias_cultivo_FLORACION);
				MICRO_SD.println(dias_cultivo_COSECHA);
				MICRO_SD.println(dias_cultivo_MANICURA_mas_REVEGETACION);
				MICRO_SD.println(dias_cultivo_SECADO_mas_REVEGETACION);
				MICRO_SD.println(dias_cultivo_CURADO_mas_REVEGETACION);
				MICRO_SD.println(dias_cultivo_RE_FLORACION);
				MICRO_SD.println(dias_cultivo_RE_COSECHA);
				MICRO_SD.println(dias_cultivo_RE_MANICURA);
				MICRO_SD.println(dias_cultivo_RE_SECADO);
				MICRO_SD.println(dias_cultivo_RE_CURADO);
				MICRO_SD.println(week_cultivo_actual_ideal);
				MICRO_SD.println(contador_dias_week_cultivo_actual_ideal);
				MICRO_SD.println(fotoperiodo_optimo_ON);
				MICRO_SD.println(fotoperiodo_optimo_OFF);
				MICRO_SD.println(temp_sol_nut_optimo);
				MICRO_SD.println(temp_amb_dia_optimo);
				MICRO_SD.println(temp_amb_noche_optimo);
				MICRO_SD.println(hum_amb_dia_optimo);
				MICRO_SD.println(hum_amb_noche_optimo);
				MICRO_SD.println(PH_sol_nut_optimo_MIN);
				MICRO_SD.println(PH_sol_nut_optimo_MAX);
				MICRO_SD.println((int)(PPM_sol_nut_optimo_MIN));
				MICRO_SD.println((int)(PPM_sol_nut_optimo_MAX));
				MICRO_SD.println(recirculacion_sol_nut_ON_optimo);
				MICRO_SD.println(recirculacion_sol_nut_OFF_optimo);
				MICRO_SD.println(recambio_aire_ON_optimo);
				MICRO_SD.println(recambio_aire_OFF_optimo);
				MICRO_SD.println(PROTECTOR_DESBALANCE_SOL_NUT);
				MICRO_SD.println(fotoperiodo_actual);
				MICRO_SD.println(aux_ciclo_peltier_1);
				MICRO_SD.println(aux_ciclo_peltier_2);
				MICRO_SD.println(aux_ciclo_peltier_3);
				MICRO_SD.println(aux_ciclo_recirculacion_sol_nut_1);
				MICRO_SD.println(aux_ciclo_recirculacion_sol_nut_2);
				MICRO_SD.println(aux_ciclo_recirculacion_sol_nut_3);
				MICRO_SD.println(aux_ciclo_recambio_aire_1);
				MICRO_SD.println(aux_ciclo_recambio_aire_2);
				MICRO_SD.println(aux_ciclo_recambio_aire_3);
				MICRO_SD.println(aux_ciclo_periodo_luz_1);
				MICRO_SD.println(aux_ciclo_periodo_luz_2);
				MICRO_SD.println(aux_ciclo_periodo_luz_3);
				MICRO_SD.println(aux_stop_compuerta_aire);
				MICRO_SD.println((int)(V_EC_mas));
				MICRO_SD.println((int)(V_EC_menos));
				MICRO_SD.println(millis_V_EC_mas);
				MICRO_SD.println(millis_V_EC_menos);
				MICRO_SD.println(aux_activar_control_PH);
				MICRO_SD.println(LED);
				MICRO_SD.println(BOMBA_RECIRCULACION_SOL_NUT);
				MICRO_SD.println(BOMBA_AIRE_OXIGENACION_SOL_NUT);
				MICRO_SD.println(BOMBA_LECTURA_PH_EC);
				MICRO_SD.println(VALVULA_PH_mas);
				MICRO_SD.println(VALVULA_PH_menos);
				MICRO_SD.println(TERMOVENTILADOR);
				MICRO_SD.println(HUMIDIFICADOR);
				MICRO_SD.println(old_pin_36);
				MICRO_SD.println(DISIPADOR_PELTIER_DESHUMIDIFICADOR);
				MICRO_SD.println(CALEFACTOR_SOL_NUT);
				MICRO_SD.println(BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT);
				MICRO_SD.println(VENTILADORES_RECAMBIO_AIRE);
				MICRO_SD.println(VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT);
				MICRO_SD.println(PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT);
				MICRO_SD.println(PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT);
				MICRO_SD.println(FUENTE_12V48A);
				MICRO_SD.println(NC_FIX);
				MICRO_SD.println(PELTIER_DESHUMIDIFICADOR);
				MICRO_SD.println(VENTILADOR_PRINCIPAL);
				MICRO_SD.println(COMPUERTA_AIRE);
				MICRO_SD.println(I2C_BUSY);
				MICRO_SD.println(mensaje_por_enviar_al_NodeMcu);
				MICRO_SD.println( String( String(char_mensaje_recibido_del_NodeMcu[0]) + 
										  String(char_mensaje_recibido_del_NodeMcu[1]) + 
										  String(char_mensaje_recibido_del_NodeMcu[2]) + 
										  String(char_mensaje_recibido_del_NodeMcu[3]) + 
										  String(char_mensaje_recibido_del_NodeMcu[4]) + 
										  String(char_mensaje_recibido_del_NodeMcu[5]) + 
										  String(char_mensaje_recibido_del_NodeMcu[6]) + 
										  String(char_mensaje_recibido_del_NodeMcu[7]) ) );
				MICRO_SD.println( String( String(char_mensaje_por_enviar_al_NodeMcu[0]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[1]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[2]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[3]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[4]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[5]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[6]) + 
										  String(char_mensaje_por_enviar_al_NodeMcu[7]) ) );
				MICRO_SD.println(hora_anterior);
				MICRO_SD.println(temp_amb_1);
				MICRO_SD.println(temp_amb_2);
				MICRO_SD.println(temp_amb_3);
				MICRO_SD.println(temp_amb_4);
				MICRO_SD.println(hum_amb_1);
				MICRO_SD.println(hum_amb_2);
				MICRO_SD.println(hum_amb_3);
				MICRO_SD.println(hum_amb_4);
				MICRO_SD.println(temp_sol_nut);
				MICRO_SD.println(temp_deshumidificador);
				MICRO_SD.println(activar_sensor_temp_deshumidificador);
				MICRO_SD.println(sensor_nivel_1_RECP_EC_menos);
				MICRO_SD.println((int)(sensor_PPM_nivel_2_RECP_EC_menos));
				MICRO_SD.println(sensor_nivel_1_RECP_EC_mas);
				MICRO_SD.println((int)(sensor_PPM_nivel_2_RECP_EC_mas));
				MICRO_SD.println(aux_old_sistema_humidificador);
				MICRO_SD.println(sensor_nivel_aprox_RECP_HUM);
				MICRO_SD.println(sensor_nivel_1_RECP_SOL_NUT);
				MICRO_SD.println(sensor_nivel_2_RECP_SOL_NUT);
				MICRO_SD.println(sensor_nivel_RECP_PH_mas);
				MICRO_SD.println(sensor_nivel_RECP_PH_menos);
				MICRO_SD.println((int)(PPM_sol_nut));
				MICRO_SD.println(aux_inicio_EC_BOMBA_LECTURA_PH_EC);
				MICRO_SD.println(aux_lectura_EC);
				MICRO_SD.println(PH_sol_nut);
				MICRO_SD.println(puerta_abierta);
				MICRO_SD.println(aux_alerta_puerta_abierta);
				MICRO_SD.println(boton_lectura_ph_presionado);
				MICRO_SD.println(activar_boton_lectura_ph);
				MICRO_SD.println(aux_temp_sol_nut);
				MICRO_SD.println((int)(volumen_ramas_bajo_sensor_2));
				MICRO_SD.println((int)(volumen_ramas_entre_sensor_1_y_2));
				MICRO_SD.println(aux_sensor_nivel_aprox_RECP_HUM);
				MICRO_SD.println(sensor_nivel_aprox_RECP_HUM);

				MICRO_SD.close();

				digitalWrite(A11, LOW);
				delay(100);
			   }
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- LEER BACKUP DESDE MICRO SD
void ReadData()
				{
				 digitalWrite(A11, HIGH);
				 delay(100);

				 MICRO_SD = SD.open("datos.txt");
				 PUNTERO_ARCHIVO_MICRO_SD = 1;
				 
				 while(MICRO_SD.available())
											{
											 String list = MICRO_SD.readStringUntil('\r');

											 if(PUNTERO_ARCHIVO_MICRO_SD == 1)
																			 {
																			  CULTIVO_INICIADO = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 2)
																			 {
																			  CONFIRMACION_INICIO_CULTIVO = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 3)
																			 {
																			  CONFIRMACION_AVANCE_ETAPA_DE_CULTIVO = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 4)
																			 {
																			  horas_cultivo_total = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 5)
																			 {
																			  dias_cultivo_total = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 6)
																			 {
																			  valor_anterior_dias_cultivo_total = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 7)
																			 {
																			  dias_cultivo_GERMINACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 8)
																			 {
																			  dias_cultivo_PLANTULA = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 9)
																			 {
																			  dias_cultivo_VEGETACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 10)
																			 {
																			  dias_cultivo_FLORACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 11)
																			 {
																			  dias_cultivo_COSECHA = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 12)
																			 {
																			  dias_cultivo_MANICURA_mas_REVEGETACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 13)
																			 {
																			  dias_cultivo_SECADO_mas_REVEGETACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 14)
																			 {
																			  dias_cultivo_CURADO_mas_REVEGETACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 15)
																			 {
																			  dias_cultivo_RE_FLORACION = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 16)
																			 {
																			  dias_cultivo_RE_COSECHA = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 17)
																			 {
																			  dias_cultivo_RE_MANICURA = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 18)
																			 {
																			  dias_cultivo_RE_SECADO = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 19)
																			 {
																			  dias_cultivo_RE_CURADO = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 20)
																			 {
																			  week_cultivo_actual_ideal = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 21)
																			 {
																			  contador_dias_week_cultivo_actual_ideal = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 22)
																			 {
																			  fotoperiodo_optimo_ON = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 23)
																			 {
																			  fotoperiodo_optimo_OFF = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 24)
																			 {
																			  temp_sol_nut_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 25)
																			 {
																			  temp_amb_dia_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 26)
																			 {
																			  temp_amb_noche_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 27)
																			 {
																			  hum_amb_dia_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 28)
																			 {
																			  hum_amb_noche_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 29)
																			 {
																			  PH_sol_nut_optimo_MIN = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 30)
																			 {
																			  PH_sol_nut_optimo_MAX = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 31)
																			 {
																			  PPM_sol_nut_optimo_MIN = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 32)
																			 {
																			  PPM_sol_nut_optimo_MAX = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 33)
																			 {
																			  recirculacion_sol_nut_ON_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 34)
																			 {
																			  recirculacion_sol_nut_OFF_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 35)
																			 {
																			  recambio_aire_ON_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 36)
																			 {
																			  recambio_aire_OFF_optimo = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 37)
																			 {
																			  PROTECTOR_DESBALANCE_SOL_NUT = list;
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 38)
																			 {
																			  fotoperiodo_actual = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 39)
																			 {
																			  aux_ciclo_peltier_1 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 40)
																			 {
																			  aux_ciclo_peltier_2 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 41)
																			 {
																			  aux_ciclo_peltier_3 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 42)
																			 {
																			  aux_ciclo_recirculacion_sol_nut_1 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 43)
																			 {
																			  aux_ciclo_recirculacion_sol_nut_2 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 44)
																			 {
																			  aux_ciclo_recirculacion_sol_nut_3 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 45)
																			 {
																			  aux_ciclo_recambio_aire_1 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 46)
																			 {
																			  aux_ciclo_recambio_aire_2 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 47)
																			 {
																			  aux_ciclo_recambio_aire_3 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 48)
																			 {
																			  aux_ciclo_periodo_luz_1 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 49)
																			 {
																			  aux_ciclo_periodo_luz_2 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 50)
																			 {
																			  aux_ciclo_periodo_luz_3 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 51)
																			 {
																			  aux_stop_compuerta_aire = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 52)
																			 {
																			  V_EC_mas = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 53)
																			 {
																			  V_EC_menos = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 54)
																			 {
																			  millis_V_EC_mas = list;
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 55)
																			 {
																			  millis_V_EC_menos = list;
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 56)
																			 {
																			  aux_activar_control_PH = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 57)
																			 {
																			  LED = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 58)
																			 {
																			  BOMBA_RECIRCULACION_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 59)
																			 {
																			  BOMBA_AIRE_OXIGENACION_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 60)
																			 {
																			  BOMBA_LECTURA_PH_EC = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 61)
																			 {
																			  VALVULA_PH_mas = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 62)
																			 {
																			  VALVULA_PH_menos = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 63)
																			 {
																			  TERMOVENTILADOR = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 64)
																			 {
																			  HUMIDIFICADOR = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 65)
																			 {
																			  old_pin_36 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 66)
																			 {
																			  DISIPADOR_PELTIER_DESHUMIDIFICADOR = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 67)
																			 {
																			  CALEFACTOR_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 68)
																			 {
																			  BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 69)
																			 {
																			  VENTILADORES_RECAMBIO_AIRE = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 70)
																			 {
																			  VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 71)
																			 {
																			  PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 72)
																			 {
																			  PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 73)
																			 {
																			  FUENTE_12V48A = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 74)
																			 {
																			  NC_FIX = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 75)
																			 {
																			  PELTIER_DESHUMIDIFICADOR = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 76)
																			 {
																			  VENTILADOR_PRINCIPAL = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 77)
																			 {
																			  COMPUERTA_AIRE = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 78)
																			 {
																			  I2C_BUSY = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 79)
																			 {
																			  mensaje_por_enviar_al_NodeMcu = list;
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 80)
																			 {
																			  char_mensaje_recibido_del_NodeMcu[0] = list.charAt(0);
																			  char_mensaje_recibido_del_NodeMcu[1] = list.charAt(1);
																			  char_mensaje_recibido_del_NodeMcu[2] = list.charAt(2);
																			  char_mensaje_recibido_del_NodeMcu[3] = list.charAt(3);
																			  char_mensaje_recibido_del_NodeMcu[4] = list.charAt(4);
																			  char_mensaje_recibido_del_NodeMcu[5] = list.charAt(5);
																			  char_mensaje_recibido_del_NodeMcu[6] = list.charAt(6);
																			  char_mensaje_recibido_del_NodeMcu[7] = list.charAt(7);
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 81)
																			 {
																			  char_mensaje_por_enviar_al_NodeMcu[0] = list.charAt(0);
																			  char_mensaje_por_enviar_al_NodeMcu[1] = list.charAt(1);
																			  char_mensaje_por_enviar_al_NodeMcu[2] = list.charAt(2);
																			  char_mensaje_por_enviar_al_NodeMcu[3] = list.charAt(3);
																			  char_mensaje_por_enviar_al_NodeMcu[4] = list.charAt(4);
																			  char_mensaje_por_enviar_al_NodeMcu[5] = list.charAt(5);
																			  char_mensaje_por_enviar_al_NodeMcu[6] = list.charAt(6);
																			  char_mensaje_por_enviar_al_NodeMcu[7] = list.charAt(7);
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 82)
																			 {
																			  hora_anterior = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 83)
																			 {
																			  temp_amb_1 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 84)
																			 {
																			  temp_amb_2 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 85)
																			 {
																			  temp_amb_3 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 86)
																			 {
																			  temp_amb_4 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 87)
																			 {
																			  hum_amb_1 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 88)
																			 {
																			  hum_amb_2 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 89)
																			 {
																			  hum_amb_3 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 90)
																			 {
																			  hum_amb_4 = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 91)
																			 {
																			  temp_sol_nut = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 92)
																			 {
																			  temp_deshumidificador = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 93)
																			 {
																			  activar_sensor_temp_deshumidificador = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 94)
																			 {
																			  sensor_nivel_1_RECP_EC_menos = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 95)
																			 {
																			  sensor_PPM_nivel_2_RECP_EC_menos = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 96)
																			 {
																			  sensor_nivel_1_RECP_EC_mas = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 97)
																			 {
																			  sensor_PPM_nivel_2_RECP_EC_mas = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 98)
																			 {
																			  aux_old_sistema_humidificador = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 99)
																			 {
																			  sensor_nivel_aprox_RECP_HUM = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 100)
																			 {
																			  sensor_nivel_1_RECP_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 101)
																			 {
																			  sensor_nivel_2_RECP_SOL_NUT = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 102)
																			 {
																			  sensor_nivel_RECP_PH_mas = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 103)
																			 {
																			  sensor_nivel_RECP_PH_menos = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 104)
																			 {
																			  PPM_sol_nut = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 105)
																			 {
																			  aux_inicio_EC_BOMBA_LECTURA_PH_EC = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 106)
																			 {
																			  aux_lectura_EC = (unsigned long)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 107)
																			 {
																			  PH_sol_nut = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 108)
																			 {
																			  puerta_abierta = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 109)
																			 {
																			  aux_alerta_puerta_abierta = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 110)
																			 {
																			  boton_lectura_ph_presionado = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 111)
																			 {
																			  activar_boton_lectura_ph = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 112)
																			 {
																			  aux_temp_sol_nut = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 113)
																			 {
																			  volumen_ramas_bajo_sensor_2 = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 114)
																			 {
																			  volumen_ramas_entre_sensor_1_y_2 = (float)(list.toInt());
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 115)
																			 {
																			  aux_sensor_nivel_aprox_RECP_HUM = list.toInt();
																			 }
											 else if(PUNTERO_ARCHIVO_MICRO_SD == 116)
																			 {
																			  sensor_nivel_aprox_RECP_HUM = list.toInt();
																			 }

											 PUNTERO_ARCHIVO_MICRO_SD++;
											}
				 MICRO_SD.close();

				 digitalWrite(A11, LOW);
				 delay(100);
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INTERRUPTOR
void INTERRUPTOR(String objeto, int estado)
											{
											if(objeto == "LED" && estado != LED)
																{
																 LED = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_LED, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_LED, HIGH);
																						}
																}
											else if(objeto == "BOMBA_RECIRCULACION_SOL_NUT" && estado != BOMBA_RECIRCULACION_SOL_NUT)
																{
																 BOMBA_RECIRCULACION_SOL_NUT = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_BOMBA_RECIRCULACION_SOL_NUT, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_BOMBA_RECIRCULACION_SOL_NUT, HIGH);
																						}
																}
											else if(objeto == "BOMBA_AIRE_OXIGENACION_SOL_NUT" && estado != BOMBA_AIRE_OXIGENACION_SOL_NUT)
																{
																 BOMBA_AIRE_OXIGENACION_SOL_NUT = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_BOMBA_AIRE_OXIGENACION_SOL_NUT, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_BOMBA_AIRE_OXIGENACION_SOL_NUT, HIGH);
																						}
																}
											else if(objeto == "BOMBA_LECTURA_PH_EC" && estado != BOMBA_LECTURA_PH_EC)
																{
																 BOMBA_LECTURA_PH_EC = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_BOMBA_LECTURA_PH_EC, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_BOMBA_LECTURA_PH_EC, HIGH);
																						}
																}
											else if(objeto == "VALVULA_PH_mas" && estado != VALVULA_PH_mas)
																{
																 VALVULA_PH_mas = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_VALVULA_PH_mas, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_VALVULA_PH_mas, HIGH);
																						}
																}
											else if(objeto == "VALVULA_PH_menos" && estado != VALVULA_PH_menos)
																{
																 VALVULA_PH_menos = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_VALVULA_PH_menos, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_VALVULA_PH_menos, HIGH);
																						}
																}
											else if(objeto == "TERMOVENTILADOR" && estado != TERMOVENTILADOR)
																{
																 TERMOVENTILADOR = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_TERMOVENTILADOR, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_TERMOVENTILADOR, HIGH);
																						}
																}
											else if(objeto == "HUMIDIFICADOR" && estado != HUMIDIFICADOR)
																{
																 HUMIDIFICADOR = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_HUMIDIFICADOR, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_HUMIDIFICADOR, HIGH);
																						}
																}
											else if(objeto == "DISIPADOR_PELTIER_DESHUMIDIFICADOR" && estado != DISIPADOR_PELTIER_DESHUMIDIFICADOR)
																{
																 DISIPADOR_PELTIER_DESHUMIDIFICADOR = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_DISIPADOR_PELTIER_DESHUMIDIFICADOR, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_DISIPADOR_PELTIER_DESHUMIDIFICADOR, HIGH);
																						}
																}
											else if(objeto == "CALEFACTOR_SOL_NUT" && estado != CALEFACTOR_SOL_NUT)
																{
																 CALEFACTOR_SOL_NUT = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_CALEFACTOR_SOL_NUT, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_CALEFACTOR_SOL_NUT, HIGH);
																						}
																}
											else if(objeto == "BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT" && estado != BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT)
																{
																 BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT  = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
																						}
																}
											else if(objeto == "VENTILADORES_RECAMBIO_AIRE" && estado != VENTILADORES_RECAMBIO_AIRE)
																{
																 VENTILADORES_RECAMBIO_AIRE = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_VENTILADORES_RECAMBIO_AIRE, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_VENTILADORES_RECAMBIO_AIRE, HIGH);
																						}
																}
											else if(objeto == "VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT" && estado != VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT)
																{
																 VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
																						}
																}
											else if(objeto == "PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT" && estado != PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT)
																{
																 PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
																						}
																}
											else if(objeto == "PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT" && estado != PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT)
																{
																 PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
																						}
																}
											else if(objeto == "NC_FIX" && estado != NC_FIX)
																{
																 NC_FIX = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_NC_FIX, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_NC_FIX, HIGH);
																						}
																}
											else if(objeto == "PELTIER_DESHUMIDIFICADOR" && estado != PELTIER_DESHUMIDIFICADOR)
																{
																 PELTIER_DESHUMIDIFICADOR = estado;
																 String aux_1 = String(estado);
																 I2C_perifericos("PDHU", aux_1);
																}
											else if(objeto == "VENTILADOR_PRINCIPAL" && estado != VENTILADOR_PRINCIPAL)
																{
																 VENTILADOR_PRINCIPAL = estado;
																 String aux_2 = String(estado);
																 I2C_perifericos("VEPR", aux_2);
																}
											else if(objeto == "COMPUERTA_AIRE" && estado != COMPUERTA_AIRE)
																{
																 COMPUERTA_AIRE = estado;
																 String aux_3 = String(estado);
																 I2C_perifericos("COAI", aux_3);
																}
											else if(objeto == "TODO" && estado == OFF)
																{
																 LED = OFF;
																 BOMBA_RECIRCULACION_SOL_NUT = OFF;
																 BOMBA_AIRE_OXIGENACION_SOL_NUT = OFF;
																 BOMBA_LECTURA_PH_EC = OFF;
																 VALVULA_PH_mas = OFF;
																 VALVULA_PH_menos = OFF;
																 TERMOVENTILADOR = OFF;
																 HUMIDIFICADOR = OFF;
																 DISIPADOR_PELTIER_DESHUMIDIFICADOR = OFF;
																 CALEFACTOR_SOL_NUT = OFF;
																 BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
																 VENTILADORES_RECAMBIO_AIRE = OFF;
																 VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
																 PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
																 PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT = OFF;
																 NC_FIX = OFF;
																 PELTIER_DESHUMIDIFICADOR = pwm_OFF;
																 VENTILADOR_PRINCIPAL = pwm_OFF;
																 COMPUERTA_AIRE = pwm_OFF;

																 digitalWrite(pin_LED, HIGH);
																 digitalWrite(pin_BOMBA_RECIRCULACION_SOL_NUT, HIGH);
																 digitalWrite(pin_BOMBA_AIRE_OXIGENACION_SOL_NUT, HIGH);
																 digitalWrite(pin_BOMBA_LECTURA_PH_EC, HIGH);
																 digitalWrite(pin_VALVULA_PH_mas, HIGH);
																 digitalWrite(pin_VALVULA_PH_menos, HIGH);
																 digitalWrite(pin_TERMOVENTILADOR, HIGH);
																 digitalWrite(pin_HUMIDIFICADOR, HIGH);
																 digitalWrite(pin_DISIPADOR_PELTIER_DESHUMIDIFICADOR, HIGH);
																 digitalWrite(pin_CALEFACTOR_SOL_NUT, HIGH);
																 digitalWrite(pin_BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
																 digitalWrite(pin_VENTILADORES_RECAMBIO_AIRE, HIGH);
																 digitalWrite(pin_VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
																 digitalWrite(pin_PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
																 digitalWrite(pin_PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
																 digitalWrite(pin_NC_FIX, HIGH);
																 I2C_perifericos("PDHU", "500");
																 I2C_perifericos("VEPR", "500");
																 I2C_perifericos("COAI", "500");
																}
											}
//____________________________________________________________________________________________________
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## SETUP
void setup()
		{
			 pinMode(A14, OUTPUT);
			 pinMode(A15, OUTPUT);
			 digitalWrite(A14, LOW);

			 pinMode(A10, OUTPUT);
			 pinMode(A11, OUTPUT);
			 digitalWrite(A10, LOW);

			 pinMode(A2, OUTPUT);
			 pinMode(A3, OUTPUT);
			 digitalWrite(A2, LOW);

			 pinMode(12, OUTPUT);
			 pinMode(13, OUTPUT);
			 digitalWrite(12, LOW);

			 //////////////////testeo



			 Serial.begin(9600);
			 delay(6000);

			 aux_frecuencia_1 = 0;
			 aux_frecuencia_2 = 0;
			 aux_frecuencia_3 = 0;
			 aux_frecuencia_4 = 0;
			 aux_frecuencia_5 = 0;
			 aux_frecuencia_6 = 0;
			 aux_frecuencia_7 = 0;
			 aux_frecuencia_8 = 0;
			 aux_frecuencia_9 = 0;
			 aux_frecuencia_10 = 0;
			 aux_frecuencia_11 = 0;
			 aux_frecuencia_12 = 0;
			 aux_frecuencia_13 = 0;
			 aux_frecuencia_14 = 0;
			 aux_frecuencia_15 = 0;
			 aux_frecuencia_16 = 0;
			 aux_frecuencia_17 = 0;
			 aux_frecuencia_18 = 0;
			 aux_frecuencia_19 = 0;
			 aux_frecuencia_20 = 0;
			 aux_frecuencia_21 = 0;
			 aux_frecuencia_22 = 0;
			 aux_frecuencia_23 = 0;
			 aux_frecuencia_24 = 0;
			 aux_frecuencia_25 = 0;
			 aux_frecuencia_26 = 0;
			 aux_frecuencia_27 = 0;
			 aux_frecuencia_28 = 0;
			 aux_frecuencia_34 = 0;
			 aux_frecuencia_35 = 0;
			 aux_frecuencia_36 = 0;
			 aux_frecuencia_37 = 0;
			 aux_frecuencia_38 = 0;
			 aux_frecuencia_39 = 0;
			 aux_frecuencia_40 = 0;
			 aux_frecuencia_41 = 0;
			 aux_frecuencia_42 = 0;
			 aux_frecuencia_43 = 0;
			 aux_frecuencia_44 = 0;
			 aux_frecuencia_45 = 0;
			 aux_frecuencia_46 = 0;
			 aux_frecuencia_47 = 0;
			 aux_frecuencia_48 = 0;
			 aux_frecuencia_49 = 0;
			 aux_frecuencia_50 = 0;
			 aux_frecuencia_51 = 0;
			 aux_frecuencia_52 = 0;
			 aux_frecuencia_53 = 0;
			 aux_frecuencia_54 = 0;
			 aux_tiempo_presionado_boton_user_friendly = 0;
			 tiempo_presionado_boton_user_friendly = 0;
			 //--------------------------------------------------------------------------------------- PINMODE PARA TODOS LOS PIN'S
			 pinMode(SD_CS, OUTPUT);

			 pinMode(pin_LED, OUTPUT);
			 digitalWrite(pin_LED, HIGH);

			 pinMode(pin_BOMBA_RECIRCULACION_SOL_NUT, OUTPUT);
			 digitalWrite(pin_BOMBA_RECIRCULACION_SOL_NUT, HIGH);

			 pinMode(pin_BOMBA_AIRE_OXIGENACION_SOL_NUT, OUTPUT);
			 digitalWrite(pin_BOMBA_AIRE_OXIGENACION_SOL_NUT, HIGH);

			 pinMode(pin_BOMBA_LECTURA_PH_EC, OUTPUT);
			 digitalWrite(pin_BOMBA_LECTURA_PH_EC, HIGH);

			 pinMode(pin_VALVULA_PH_mas, OUTPUT);
			 digitalWrite(pin_VALVULA_PH_mas, HIGH);

			 pinMode(pin_VALVULA_PH_menos, OUTPUT);
			 digitalWrite(pin_VALVULA_PH_menos, HIGH);

			 pinMode(pin_TERMOVENTILADOR, OUTPUT);
			 digitalWrite(pin_TERMOVENTILADOR, HIGH);

			 pinMode(pin_HUMIDIFICADOR, OUTPUT);
			 digitalWrite(pin_HUMIDIFICADOR, HIGH);

			 pinMode(pin_DISIPADOR_PELTIER_DESHUMIDIFICADOR, OUTPUT);
			 digitalWrite(pin_DISIPADOR_PELTIER_DESHUMIDIFICADOR, HIGH);

			 pinMode(pin_CALEFACTOR_SOL_NUT, OUTPUT);
			 digitalWrite(pin_CALEFACTOR_SOL_NUT, HIGH);

			 pinMode(pin_BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT, OUTPUT);
			 digitalWrite(pin_BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);

			 pinMode(pin_VENTILADORES_RECAMBIO_AIRE, OUTPUT);
			 digitalWrite(pin_VENTILADORES_RECAMBIO_AIRE, HIGH);

			 pinMode(pin_VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT, OUTPUT);
			 digitalWrite(pin_VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT, HIGH);
			 
			 pinMode(pin_PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT, OUTPUT);
			 digitalWrite(pin_PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);

			 pinMode(pin_PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT, OUTPUT);
			 digitalWrite(pin_PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT, LOW);
			 
			 pinMode(pin_NC_FIX, OUTPUT);
			 digitalWrite(pin_NC_FIX, HIGH);


			 pinMode(pin_boton_lectura_ph, INPUT);

			 pinMode(pin_sensor_nivel_extra_superior, INPUT);
			 pinMode(pin_sensor_nivel_1_RECP_EC_mas, INPUT);
			 pinMode(pin_sensor_nivel_2_RECP_SOL_NUT, INPUT);

			 pinMode(pin_5v_sensor_nivel_extra_superior, OUTPUT);
			 pinMode(pin_5v_sensor_nivel_1_RECP_EC_mas, OUTPUT);
			 pinMode(pin_5v_sensor_nivel_2_RECP_SOL_NUT, OUTPUT);

			 digitalWrite(pin_5v_sensor_nivel_extra_superior, LOW);
			 digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, LOW);
			 digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, LOW);
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INICIO ENLACE I2C
			 Wire.begin(12);
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INICIO SENSORES DS18b20
			 delay(1000);
			 SENSOR_TEMP_SOL_NUT.begin();
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INICIO MICRO SD
			 SD.begin(SD_CS);
			 //_______________________________________________________________________________________
			 delay(1000);
			 Serial.println("");
			 //--------------------------------------------------------------------------------------- LEER BACKUP
			 ReadData();
			 //_______________________________________________________________________________________
			 delay(1000);
			 //--------------------------------------------------------------------------------------- INTERRUPTOR ( todo , off )
			 INTERRUPTOR("TODO", OFF);
			 //_______________________________________________________________________________________
			 delay(2000);
			 //--------------------------------------------------------------------------------------- INTERRUPTOR( NC fix, on )
			 INTERRUPTOR("NC_FIX", ON);
			 //_______________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA HORA ACTUAL
			 I2C_lectura_sensor("hora");

			 hora_actual = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
								  String(char_mensaje_recibido_del_NodeMcu[6]) +
								  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
														{
														 char_mensaje_recibido_del_NodeMcu[i] =' ';
														}
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA NIVEL 2 RECP. SOL. NUT. ACTUAL
			 digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, HIGH);

			 if(digitalRead(pin_sensor_nivel_2_RECP_SOL_NUT) == LOW)
																 {
																  sensor_nivel_2_RECP_SOL_NUT = ON;
																  digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, LOW);
																 }

			 else if (digitalRead(pin_sensor_nivel_2_RECP_SOL_NUT) == HIGH)
																 {
																  sensor_nivel_2_RECP_SOL_NUT = OFF;
																  digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, LOW);
																 }
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA NIVEL 1 RECP. SOL. NUT. ACTUAL
			 I2C_lectura_sensor("n1sn");

			 sensor_nivel_1_RECP_SOL_NUT = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
												  String(char_mensaje_recibido_del_NodeMcu[6]) +
												  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
														{
														 char_mensaje_recibido_del_NodeMcu[i] =' ';
														}
			}
			//________________________________________________________________________________________			
			{//--------------------------------------------------------------------------------------- LECTURA PPM NIVEL 2 RECP. PPM-- ACTUAL
			 delay(5000);I2C_lectura_sensor("2ec-");

			 sensor_PPM_nivel_2_RECP_EC_menos = (float)((String(String(char_mensaje_recibido_del_NodeMcu[5]) +
														  String(char_mensaje_recibido_del_NodeMcu[6]) +
														  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));
														  
			 sensor_PPM_nivel_2_RECP_EC_menos = (sensor_PPM_nivel_2_RECP_EC_menos/(1+ (0.0191*(15 - 25))) )*(1/K_PPM_menos);

			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
														{
														 char_mensaje_recibido_del_NodeMcu[i] =' ';
														}
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA NIVEL 1 RECP. PPM-- ACTUAL
			 I2C_lectura_sensor("1ec-");

			 sensor_nivel_1_RECP_EC_menos = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
												   String(char_mensaje_recibido_del_NodeMcu[6]) +
												   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
														{
														 char_mensaje_recibido_del_NodeMcu[i] =' ';
														}
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA PPM NIVEL 2 RECP. PPM++ ACTUAL
			 delay(5000);I2C_lectura_sensor("2ec+");

			 sensor_PPM_nivel_2_RECP_EC_mas = (float)(String( String(char_mensaje_recibido_del_NodeMcu[5]) +
												String(char_mensaje_recibido_del_NodeMcu[6]) +
												String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt());

			 sensor_PPM_nivel_2_RECP_EC_mas = (sensor_PPM_nivel_2_RECP_EC_mas/(1+ (0.0191*(15 - 25))) )*(1/K_PPM_mas);

			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
														{
														 char_mensaje_recibido_del_NodeMcu[i] =' ';
														}
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA NIVEL 1 RECP. PPM++ ACTUAL
			 digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, HIGH);

			 if(digitalRead(pin_sensor_nivel_1_RECP_EC_mas) == LOW)
																 {
																  sensor_nivel_1_RECP_EC_mas = ON;
																  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, LOW);
																 }

			 else if (digitalRead(pin_sensor_nivel_1_RECP_EC_mas) == HIGH)
																 {
																  sensor_nivel_1_RECP_EC_mas = OFF;
																  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, LOW);
																 }
			}
			//________________________________________________________________________________________	
			{//--------------------------------------------------------------------------------------- LECTURA NIVEL RECP. PH++ ACTUAL
//			 I2C_lectura_sensor("rph+");
//
//			 sensor_nivel_RECP_PH_mas = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
//										String(char_mensaje_recibido_del_NodeMcu[6]) +
//										String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();
//
//			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
//														{
//														 char_mensaje_recibido_del_NodeMcu[i] =' ';
//														}
			}
			//_______________________________________________________________________________________	
			{//--------------------------------------------------------------------------------------- LECTURA NIVEL RECP. PH-- ACTUAL
//			 I2C_lectura_sensor("rph-");
//
//			 sensor_nivel_RECP_PH_menos = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
//										  String(char_mensaje_recibido_del_NodeMcu[6]) +
//										  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();
//
//			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
//														{
//														 char_mensaje_recibido_del_NodeMcu[i] =' ';
//														}
			}
			//_______________________________________________________________________________________			
		}
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## LOOP
void loop()
		{
			 parametros_cultivo WEEK[50];
			 delay(200);
			 I2C_tocar_cancion(mp3_no_repeat);

			{//-------------------------------------------------------------------------------------- PARAMETROS WEEK'S DE CULTIVO IDEAL

												 {// WEEK[1
												  WEEK[1].fotoperiodo = -10;          WEEK[1].etapa_cultivo = F("GER");

												  WEEK[1].temp_sol_nut = 25;

												  WEEK[1].temp_amb[dia] = 25;         WEEK[1].temp_amb[noche] = 25;

												  WEEK[1].hum_amb[dia] = 95;          WEEK[1].hum_amb[noche] = 95;

												  WEEK[1].ph_sol_nut[MIN] = 55;       WEEK[1].ph_sol_nut[MAX] = 65;

												  WEEK[1].ppm_sol_nut[MIN] = -30;        WEEK[1].ppm_sol_nut[MAX] = -30;

												  WEEK[1].recirculacion_sol_nut[ON] = 5;    WEEK[1].recirculacion_sol_nut[OFF] = 5;

												  WEEK[1].recambio_aire[ON] = 4;        WEEK[1].recambio_aire[OFF] = 60;}

												 {// WEEK[2
												  WEEK[2].fotoperiodo = 20;         WEEK[2].etapa_cultivo = F("PLA");

												  WEEK[2].temp_sol_nut = 19;

												  WEEK[2].temp_amb[dia] = 28;         WEEK[2].temp_amb[noche] = 21;

												  WEEK[2].hum_amb[dia] = 74;          WEEK[2].hum_amb[noche] = 61;

												  WEEK[2].ph_sol_nut[MIN] = 55;       WEEK[2].ph_sol_nut[MAX] = 65;

												  WEEK[2].ppm_sol_nut[MIN] = 350;        WEEK[2].ppm_sol_nut[MAX] = 400;

												  WEEK[2].recirculacion_sol_nut[ON] = 5;    WEEK[2].recirculacion_sol_nut[OFF] = 5;

												  WEEK[2].recambio_aire[ON] = 4;        WEEK[2].recambio_aire[OFF] = 60;}

												 {// WEEK[3
												  WEEK[3].fotoperiodo = 20;         WEEK[3].etapa_cultivo = F("PLA");

												  WEEK[3].temp_sol_nut = 19;

												  WEEK[3].temp_amb[dia] = 28;         WEEK[3].temp_amb[noche] = 21;

												  WEEK[3].hum_amb[dia] = 74;          WEEK[3].hum_amb[noche] = 61;

												  WEEK[3].ph_sol_nut[MIN] = 55;       WEEK[3].ph_sol_nut[MAX] = 65;

												  WEEK[3].ppm_sol_nut[MIN] = 700;        WEEK[3].ppm_sol_nut[MAX] = 875;

												  WEEK[3].recirculacion_sol_nut[ON] = 5;    WEEK[3].recirculacion_sol_nut[OFF] = 5;

												  WEEK[3].recambio_aire[ON] = 4;        WEEK[3].recambio_aire[OFF] = 60;}

												 {// WEEK[4
												  WEEK[4].fotoperiodo = 20;         WEEK[4].etapa_cultivo = F("VEG");

												  WEEK[4].temp_sol_nut = 19;

												  WEEK[4].temp_amb[dia] = 28;         WEEK[4].temp_amb[noche] = 21;

												  WEEK[4].hum_amb[dia] = 74;          WEEK[4].hum_amb[noche] = 61;

												  WEEK[4].ph_sol_nut[MIN] = 55;       WEEK[4].ph_sol_nut[MAX] = 65;

												  WEEK[4].ppm_sol_nut[MIN] = 1050;       WEEK[4].ppm_sol_nut[MAX] = 1350;

												  WEEK[4].recirculacion_sol_nut[ON] = 5;    WEEK[4].recirculacion_sol_nut[OFF] = 5;

												  WEEK[4].recambio_aire[ON] = 4;        WEEK[4].recambio_aire[OFF] = 60;}

												 {// WEEK[5
												  WEEK[5].fotoperiodo = 20;         WEEK[5].etapa_cultivo = F("VEG");

												  WEEK[5].temp_sol_nut = 19;

												  WEEK[5].temp_amb[dia] = 28;         WEEK[5].temp_amb[noche] = 21;

												  WEEK[5].hum_amb[dia] = 74;          WEEK[5].hum_amb[noche] = 61;

												  WEEK[5].ph_sol_nut[MIN] = 55;       WEEK[5].ph_sol_nut[MAX] = 65;

												  WEEK[5].ppm_sol_nut[MIN] = 1050;       WEEK[5].ppm_sol_nut[MAX] = 1350;

												  WEEK[5].recirculacion_sol_nut[ON] = 5;    WEEK[5].recirculacion_sol_nut[OFF] = 5;

												  WEEK[5].recambio_aire[ON] = 4;        WEEK[5].recambio_aire[OFF] = 60;}

												 {// WEEK[6
												  WEEK[6].fotoperiodo = 20;         WEEK[6].etapa_cultivo = F("VEG");

												  WEEK[6].temp_sol_nut = 19;

												  WEEK[6].temp_amb[dia] = 28;         WEEK[6].temp_amb[noche] = 21;

												  WEEK[6].hum_amb[dia] = 74;          WEEK[6].hum_amb[noche] = 61;

												  WEEK[6].ph_sol_nut[MIN] = 55;       WEEK[6].ph_sol_nut[MAX] = 65;

												  WEEK[6].ppm_sol_nut[MIN] = 1050;       WEEK[6].ppm_sol_nut[MAX] = 1350;

												  WEEK[6].recirculacion_sol_nut[ON] = 5;    WEEK[6].recirculacion_sol_nut[OFF] = 5;

												  WEEK[6].recambio_aire[ON] = 4;        WEEK[6].recambio_aire[OFF] = 60;}

												 {// WEEK[7
												  WEEK[7].fotoperiodo = 20;         WEEK[7].etapa_cultivo = F("VEG");

												  WEEK[7].temp_sol_nut = 19;

												  WEEK[7].temp_amb[dia] = 28;         WEEK[7].temp_amb[noche] = 21;

												  WEEK[7].hum_amb[dia] = 74;          WEEK[7].hum_amb[noche] = 61;

												  WEEK[7].ph_sol_nut[MIN] = 55;       WEEK[7].ph_sol_nut[MAX] = 65;

												  WEEK[7].ppm_sol_nut[MIN] = 1050;       WEEK[7].ppm_sol_nut[MAX] = 1350;

												  WEEK[7].recirculacion_sol_nut[ON] = 5;    WEEK[7].recirculacion_sol_nut[OFF] = 5;

												  WEEK[7].recambio_aire[ON] = 4;        WEEK[2].recambio_aire[OFF] = 60;}

												 {// WEEK[8
												  WEEK[8].fotoperiodo = 20;         WEEK[8].etapa_cultivo = F("VEG");

												  WEEK[8].temp_sol_nut = 19;

												  WEEK[8].temp_amb[dia] = 28;         WEEK[8].temp_amb[noche] = 21;

												  WEEK[8].hum_amb[dia] = 74;          WEEK[8].hum_amb[noche] = 61;

												  WEEK[8].ph_sol_nut[MIN] = 55;       WEEK[8].ph_sol_nut[MAX] = 65;

												  WEEK[8].ppm_sol_nut[MIN] = 1050;       WEEK[8].ppm_sol_nut[MAX] = 1350;

												  WEEK[8].recirculacion_sol_nut[ON] = 5;    WEEK[8].recirculacion_sol_nut[OFF] = 5;

												  WEEK[8].recambio_aire[ON] = 4;        WEEK[8].recambio_aire[OFF] = 60;}

												 {// WEEK[9
												  WEEK[9].fotoperiodo = 20;         WEEK[9].etapa_cultivo = F("VEG");

												  WEEK[9].temp_sol_nut = 19;

												  WEEK[9].temp_amb[dia] = 28;         WEEK[9].temp_amb[noche] = 21;

												  WEEK[9].hum_amb[dia] = 74;          WEEK[9].hum_amb[noche] = 61;

												  WEEK[9].ph_sol_nut[MIN] = 55;       WEEK[9].ph_sol_nut[MAX] = 65;

												  WEEK[9].ppm_sol_nut[MIN] = 1050;       WEEK[9].ppm_sol_nut[MAX] = 1350;

												  WEEK[9].recirculacion_sol_nut[ON] = 5;    WEEK[9].recirculacion_sol_nut[OFF] = 5;

												  WEEK[9].recambio_aire[ON] = 4;        WEEK[9].recambio_aire[OFF] = 60;}

												 {// WEEK[10
												  WEEK[10].fotoperiodo = 20;          WEEK[10].etapa_cultivo = F("VEG");

												  WEEK[10].temp_sol_nut = 19;

												  WEEK[10].temp_amb[dia] = 28;        WEEK[10].temp_amb[noche] = 21;

												  WEEK[10].hum_amb[dia] = 74;         WEEK[10].hum_amb[noche] = 61;

												  WEEK[10].ph_sol_nut[MIN] = 55;        WEEK[10].ph_sol_nut[MAX] = 65;

												  WEEK[10].ppm_sol_nut[MIN] = 950;       WEEK[10].ppm_sol_nut[MAX] = 1400;

												  WEEK[10].recirculacion_sol_nut[ON] = 5;   WEEK[10].recirculacion_sol_nut[OFF] = 5;

												  WEEK[10].recambio_aire[ON] = 4;       WEEK[10].recambio_aire[OFF] = 60;}

												 {// WEEK[11
												  WEEK[11].fotoperiodo = 20;          WEEK[11].etapa_cultivo = F("VEG");

												  WEEK[11].temp_sol_nut = 19;

												  WEEK[11].temp_amb[dia] = 28;        WEEK[11].temp_amb[noche] = 21;

												  WEEK[11].hum_amb[dia] = 74;         WEEK[11].hum_amb[noche] = 61;

												  WEEK[11].ph_sol_nut[MIN] = 55;        WEEK[11].ph_sol_nut[MAX] = 65;

												  WEEK[11].ppm_sol_nut[MIN] = 950;       WEEK[11].ppm_sol_nut[MAX] = 1400;

												  WEEK[11].recirculacion_sol_nut[ON] = 5;   WEEK[11].recirculacion_sol_nut[OFF] = 5;

												  WEEK[11].recambio_aire[ON] = 4;       WEEK[11].recambio_aire[OFF] = 60;}

												 {// WEEK[12
												  WEEK[12].fotoperiodo = 20;          WEEK[12].etapa_cultivo = F("VEG");

												  WEEK[12].temp_sol_nut = 19;

												  WEEK[12].temp_amb[dia] = 28;        WEEK[12].temp_amb[noche] = 21;

												  WEEK[12].hum_amb[dia] = 74;         WEEK[12].hum_amb[noche] = 61;

												  WEEK[12].ph_sol_nut[MIN] = 55;        WEEK[12].ph_sol_nut[MAX] = 65;

												  WEEK[12].ppm_sol_nut[MIN] = 950;       WEEK[12].ppm_sol_nut[MAX] = 1400;

												  WEEK[12].recirculacion_sol_nut[ON] = 5;   WEEK[12].recirculacion_sol_nut[OFF] = 5;

												  WEEK[12].recambio_aire[ON] = 4;       WEEK[12].recambio_aire[OFF] = 60;}

												 {// WEEK[13
												  WEEK[13].fotoperiodo = 20;          WEEK[13].etapa_cultivo = F("VEG");

												  WEEK[13].temp_sol_nut = 19;

												  WEEK[13].temp_amb[dia] = 28;        WEEK[13].temp_amb[noche] = 21;

												  WEEK[13].hum_amb[dia] = 74;         WEEK[13].hum_amb[noche] = 61;

												  WEEK[13].ph_sol_nut[MIN] = 55;        WEEK[13].ph_sol_nut[MAX] = 65;

												  WEEK[13].ppm_sol_nut[MIN] = 950;       WEEK[13].ppm_sol_nut[MAX] = 1400;

												  WEEK[13].recirculacion_sol_nut[ON] = 5;   WEEK[13].recirculacion_sol_nut[OFF] = 5;

												  WEEK[13].recambio_aire[ON] = 4;       WEEK[13].recambio_aire[OFF] = 60;}

												 {// WEEK[14
												  WEEK[14].fotoperiodo = 20;          WEEK[14].etapa_cultivo = F("VEG");

												  WEEK[14].temp_sol_nut = 19;

												  WEEK[14].temp_amb[dia] = 28;        WEEK[14].temp_amb[noche] = 21;

												  WEEK[14].hum_amb[dia] = 74;         WEEK[14].hum_amb[noche] = 61;

												  WEEK[14].ph_sol_nut[MIN] = 55;        WEEK[14].ph_sol_nut[MAX] = 65;

												  WEEK[14].ppm_sol_nut[MIN] = 950;       WEEK[14].ppm_sol_nut[MAX] = 1400;

												  WEEK[14].recirculacion_sol_nut[ON] = 5;   WEEK[14].recirculacion_sol_nut[OFF] = 5;

												  WEEK[14].recambio_aire[ON] = 4;       WEEK[14].recambio_aire[OFF] = 60;}

												 {// WEEK[15
												  WEEK[15].fotoperiodo = 20;          WEEK[15].etapa_cultivo = F("VEG");

												  WEEK[15].temp_sol_nut = 19;

												  WEEK[15].temp_amb[dia] = 28;        WEEK[15].temp_amb[noche] = 21;

												  WEEK[15].hum_amb[dia] = 74;         WEEK[15].hum_amb[noche] = 61;

												  WEEK[15].ph_sol_nut[MIN] = 55;        WEEK[15].ph_sol_nut[MAX] = 65;

												  WEEK[15].ppm_sol_nut[MIN] = 950;       WEEK[15].ppm_sol_nut[MAX] = 1400;

												  WEEK[15].recirculacion_sol_nut[ON] = 5;   WEEK[15].recirculacion_sol_nut[OFF] = 5;

												  WEEK[15].recambio_aire[ON] = 4;       WEEK[15].recambio_aire[OFF] = 60;}

												 {// WEEK[16
												  WEEK[16].fotoperiodo = 12;          WEEK[16].etapa_cultivo = F("FLO");

												  WEEK[16].temp_sol_nut = 19;

												  WEEK[16].temp_amb[dia] = 28;        WEEK[16].temp_amb[noche] = 21;

												  WEEK[16].hum_amb[dia] = 68;         WEEK[16].hum_amb[noche] = 52;

												  WEEK[16].ph_sol_nut[MIN] = 55;        WEEK[16].ph_sol_nut[MAX] = 65;

												  WEEK[16].ppm_sol_nut[MIN] = 900;       WEEK[16].ppm_sol_nut[MAX] = 1300;

												  WEEK[16].recirculacion_sol_nut[ON] = 5;   WEEK[16].recirculacion_sol_nut[OFF] = 5;

												  WEEK[16].recambio_aire[ON] = 4;       WEEK[16].recambio_aire[OFF] = 60;}

												 {// WEEK[17
												  WEEK[17].fotoperiodo = 12;          WEEK[17].etapa_cultivo = F("FLO");

												  WEEK[17].temp_sol_nut = 19;

												  WEEK[17].temp_amb[dia] = 28;        WEEK[17].temp_amb[noche] = 21;

												  WEEK[17].hum_amb[dia] = 68;         WEEK[17].hum_amb[noche] = 52;

												  WEEK[17].ph_sol_nut[MIN] = 55;        WEEK[17].ph_sol_nut[MAX] = 65;

												  WEEK[17].ppm_sol_nut[MIN] = 950;       WEEK[17].ppm_sol_nut[MAX] = 1300;

												  WEEK[17].recirculacion_sol_nut[ON] = 5;   WEEK[17].recirculacion_sol_nut[OFF] = 5;

												  WEEK[17].recambio_aire[ON] = 4;       WEEK[17].recambio_aire[OFF] = 60;}

												 {// WEEK[18
												  WEEK[18].fotoperiodo = 12;          WEEK[18].etapa_cultivo = F("FLO");

												  WEEK[18].temp_sol_nut = 19;

												  WEEK[18].temp_amb[dia] = 28;        WEEK[18].temp_amb[noche] = 21;

												  WEEK[18].hum_amb[dia] = 68;         WEEK[18].hum_amb[noche] = 52;

												  WEEK[18].ph_sol_nut[MIN] = 55;        WEEK[18].ph_sol_nut[MAX] = 65;

												  WEEK[18].ppm_sol_nut[MIN] = 950;       WEEK[18].ppm_sol_nut[MAX] = 1300;

												  WEEK[18].recirculacion_sol_nut[ON] = 5;   WEEK[18].recirculacion_sol_nut[OFF] = 5;

												  WEEK[18].recambio_aire[ON] = 4;       WEEK[18].recambio_aire[OFF] = 60;}

												 {// WEEK[19
												  WEEK[19].fotoperiodo = 12;          WEEK[19].etapa_cultivo = F("FLO");

												  WEEK[19].temp_sol_nut = 19;

												  WEEK[19].temp_amb[dia] = 28;        WEEK[19].temp_amb[noche] = 21;

												  WEEK[19].hum_amb[dia] = 68;         WEEK[19].hum_amb[noche] = 52;

												  WEEK[19].ph_sol_nut[MIN] = 55;        WEEK[19].ph_sol_nut[MAX] = 65;

												  WEEK[19].ppm_sol_nut[MIN] = 1000;      WEEK[19].ppm_sol_nut[MAX] = 1400;

												  WEEK[19].recirculacion_sol_nut[ON] = 5;   WEEK[19].recirculacion_sol_nut[OFF] = 5;

												  WEEK[19].recambio_aire[ON] = 4;       WEEK[19].recambio_aire[OFF] = 60;}

												 {// WEEK[20
												  WEEK[20].fotoperiodo = 12;          WEEK[20].etapa_cultivo = F("FLO");

												  WEEK[20].temp_sol_nut = 19;

												  WEEK[20].temp_amb[dia] = 28;        WEEK[20].temp_amb[noche] = 21;

												  WEEK[20].hum_amb[dia] = 68;         WEEK[20].hum_amb[noche] = 52;

												  WEEK[20].ph_sol_nut[MIN] = 55;        WEEK[20].ph_sol_nut[MAX] = 65;

												  WEEK[20].ppm_sol_nut[MIN] = 1000;      WEEK[20].ppm_sol_nut[MAX] = 1400;

												  WEEK[20].recirculacion_sol_nut[ON] = 5;   WEEK[20].recirculacion_sol_nut[OFF] = 5;

												  WEEK[20].recambio_aire[ON] = 4;       WEEK[20].recambio_aire[OFF] = 60;}

												 {// WEEK[21
												  WEEK[21].fotoperiodo = 12;          WEEK[21].etapa_cultivo = F("FLO");

												  WEEK[21].temp_sol_nut = 19;

												  WEEK[21].temp_amb[dia] = 28;        WEEK[21].temp_amb[noche] = 21;

												  WEEK[21].hum_amb[dia] = 63;         WEEK[21].hum_amb[noche] = 44;

												  WEEK[21].ph_sol_nut[MIN] = 55;        WEEK[21].ph_sol_nut[MAX] = 65;

												  WEEK[21].ppm_sol_nut[MIN] = 1050;      WEEK[21].ppm_sol_nut[MAX] = 1400;

												  WEEK[21].recirculacion_sol_nut[ON] = 5;   WEEK[21].recirculacion_sol_nut[OFF] = 5;

												  WEEK[21].recambio_aire[ON] = 4;       WEEK[21].recambio_aire[OFF] = 60;}

												 {// WEEK[22
												  WEEK[22].fotoperiodo = 12;          WEEK[22].etapa_cultivo = F("FLO");

												  WEEK[22].temp_sol_nut = 19;

												  WEEK[22].temp_amb[dia] = 28;        WEEK[22].temp_amb[noche] = 21;

												  WEEK[22].hum_amb[dia] = 63;         WEEK[22].hum_amb[noche] = 44;

												  WEEK[22].ph_sol_nut[MIN] = 55;        WEEK[22].ph_sol_nut[MAX] = 65;

												  WEEK[22].ppm_sol_nut[MIN] = 1050;      WEEK[22].ppm_sol_nut[MAX] = 1400;

												  WEEK[22].recirculacion_sol_nut[ON] = 5;   WEEK[22].recirculacion_sol_nut[OFF] = 5;

												  WEEK[22].recambio_aire[ON] = 4;       WEEK[22].recambio_aire[OFF] = 60;}

												 {// WEEK[23
												  WEEK[23].fotoperiodo = 12;          WEEK[23].etapa_cultivo = F("FLO");

												  WEEK[23].temp_sol_nut = 19;

												  WEEK[23].temp_amb[dia] = 28;        WEEK[23].temp_amb[noche] = 21;

												  WEEK[23].hum_amb[dia] = 63;         WEEK[23].hum_amb[noche] = 44;

												  WEEK[23].ph_sol_nut[MIN] = 55;        WEEK[23].ph_sol_nut[MAX] = 65;

												  WEEK[23].ppm_sol_nut[MIN] = 1000;      WEEK[23].ppm_sol_nut[MAX] = 1300;

												  WEEK[23].recirculacion_sol_nut[ON] = 5;   WEEK[23].recirculacion_sol_nut[OFF] = 5;

												  WEEK[23].recambio_aire[ON] = 4;       WEEK[23].recambio_aire[OFF] = 60;}

												 {// WEEK[24
												  WEEK[24].fotoperiodo = 12;          WEEK[24].etapa_cultivo = F("FLO");

												  WEEK[24].temp_sol_nut = 19;

												  WEEK[24].temp_amb[dia] = 28;        WEEK[24].temp_amb[noche] = 21;

												  WEEK[24].hum_amb[dia] = 63;         WEEK[24].hum_amb[noche] = 44;

												  WEEK[24].ph_sol_nut[MIN] = 55;        WEEK[24].ph_sol_nut[MAX] = 65;

												  WEEK[24].ppm_sol_nut[MIN] = 750;       WEEK[24].ppm_sol_nut[MAX] = 1150;

												  WEEK[24].recirculacion_sol_nut[ON] = 5;   WEEK[24].recirculacion_sol_nut[OFF] = 5;

												  WEEK[24].recambio_aire[ON] = 4;       WEEK[24].recambio_aire[OFF] = 60;}

												 {// WEEK[25
												  WEEK[25].fotoperiodo = -20;         WEEK[25].etapa_cultivo = F("FLO");

												  WEEK[25].temp_sol_nut = 19;

												  WEEK[25].temp_amb[dia] = 28;        WEEK[25].temp_amb[noche] = 21;

												  WEEK[25].hum_amb[dia] = 63;         WEEK[25].hum_amb[noche] = 44;

												  WEEK[25].ph_sol_nut[MIN] = 55;        WEEK[25].ph_sol_nut[MAX] = 65;

												  WEEK[25].ppm_sol_nut[MIN] = 0;       WEEK[25].ppm_sol_nut[MAX] = 50;

												  WEEK[25].recirculacion_sol_nut[ON] = 5;   WEEK[25].recirculacion_sol_nut[OFF] = 5;

												  WEEK[25].recambio_aire[ON] = 4;       WEEK[25].recambio_aire[OFF] = 60;}

												 {// WEEK[26
												  WEEK[26].fotoperiodo = 12;          WEEK[26].etapa_cultivo = F("COS");

												  WEEK[26].temp_sol_nut = 19;

												  WEEK[26].temp_amb[dia] = 28;        WEEK[26].temp_amb[noche] = 21;

												  WEEK[26].hum_amb[dia] = 63;         WEEK[26].hum_amb[noche] = 44;

												  WEEK[26].ph_sol_nut[MIN] = 55;        WEEK[26].ph_sol_nut[MAX] = 65;

												  WEEK[26].ppm_sol_nut[MIN] = 0;       WEEK[26].ppm_sol_nut[MAX] = 50;

												  WEEK[26].recirculacion_sol_nut[ON] = 5;   WEEK[26].recirculacion_sol_nut[OFF] = 5;

												  WEEK[26].recambio_aire[ON] = 4;       WEEK[26].recambio_aire[OFF] = 60;}

												 {// WEEK[27
												  WEEK[27].fotoperiodo = 24;          WEEK[27].etapa_cultivo = F("M+R");

												  WEEK[27].temp_sol_nut = 19;

												  WEEK[27].temp_amb[dia] = 28;        WEEK[27].temp_amb[noche] = 21;

												  WEEK[27].hum_amb[dia] = 74;         WEEK[27].hum_amb[noche] = 61;

												  WEEK[27].ph_sol_nut[MIN] = 55;        WEEK[27].ph_sol_nut[MAX] = 65;

												  WEEK[27].ppm_sol_nut[MIN] = 1050;      WEEK[27].ppm_sol_nut[MAX] = 1350;

												  WEEK[27].recirculacion_sol_nut[ON] = 5;   WEEK[27].recirculacion_sol_nut[OFF] = 5;

												  WEEK[27].recambio_aire[ON] = 4;       WEEK[27].recambio_aire[OFF] = 60;}

												 {// WEEK[28
												  WEEK[28].fotoperiodo = 24;          WEEK[28].etapa_cultivo = F("S+R");

												  WEEK[28].temp_sol_nut = 19;

												  WEEK[28].temp_amb[dia] = 28;        WEEK[28].temp_amb[noche] = 21;

												  WEEK[28].hum_amb[dia] = 74;         WEEK[28].hum_amb[noche] = 61;

												  WEEK[28].ph_sol_nut[MIN] = 55;        WEEK[28].ph_sol_nut[MAX] = 65;

												  WEEK[28].ppm_sol_nut[MIN] = 1050;      WEEK[28].ppm_sol_nut[MAX] = 1350;

												  WEEK[28].recirculacion_sol_nut[ON] = 5;   WEEK[28].recirculacion_sol_nut[OFF] = 5;

												  WEEK[28].recambio_aire[ON] = 4;       WEEK[28].recambio_aire[OFF] = 60;}

												 {// WEEK[29
												  WEEK[29].fotoperiodo = 24;          WEEK[29].etapa_cultivo = F("S+R");

												  WEEK[29].temp_sol_nut = 19;

												  WEEK[29].temp_amb[dia] = 28;        WEEK[29].temp_amb[noche] = 21;

												  WEEK[29].hum_amb[dia] = 74;         WEEK[29].hum_amb[noche] = 61;

												  WEEK[29].ph_sol_nut[MIN] = 55;        WEEK[29].ph_sol_nut[MAX] = 65;

												  WEEK[29].ppm_sol_nut[MIN] = 1050;      WEEK[29].ppm_sol_nut[MAX] = 1350;

												  WEEK[29].recirculacion_sol_nut[ON] = 5;   WEEK[29].recirculacion_sol_nut[OFF] = 5;

												  WEEK[29].recambio_aire[ON] = 4;       WEEK[29].recambio_aire[OFF] = 60;}

												 {// WEEK[30
												  WEEK[30].fotoperiodo = 24;          WEEK[30].etapa_cultivo = F("S+R");

												  WEEK[30].temp_sol_nut = 19;

												  WEEK[30].temp_amb[dia] = 28;        WEEK[30].temp_amb[noche] = 21;

												  WEEK[30].hum_amb[dia] = 74;         WEEK[30].hum_amb[noche] = 61;

												  WEEK[30].ph_sol_nut[MIN] = 55;        WEEK[30].ph_sol_nut[MAX] = 65;

												  WEEK[30].ppm_sol_nut[MIN] = 1050;      WEEK[30].ppm_sol_nut[MAX] = 1350;

												  WEEK[30].recirculacion_sol_nut[ON] = 5;   WEEK[30].recirculacion_sol_nut[OFF] = 5;

												  WEEK[30].recambio_aire[ON] = 4;       WEEK[30].recambio_aire[OFF] = 60;}

												 {// WEEK[31
												  WEEK[31].fotoperiodo = 20;          WEEK[30].etapa_cultivo = F("C+R");

												  WEEK[31].temp_sol_nut = 19;

												  WEEK[31].temp_amb[dia] = 28;        WEEK[31].temp_amb[noche] = 21;

												  WEEK[31].hum_amb[dia] = 74;         WEEK[31].hum_amb[noche] = 61;

												  WEEK[31].ph_sol_nut[MIN] = 55;        WEEK[31].ph_sol_nut[MAX] = 65;

												  WEEK[31].ppm_sol_nut[MIN] = 1050;      WEEK[31].ppm_sol_nut[MAX] = 1350;

												  WEEK[31].recirculacion_sol_nut[ON] = 5;   WEEK[31].recirculacion_sol_nut[OFF] = 5;

												  WEEK[31].recambio_aire[ON] = 4;       WEEK[31].recambio_aire[OFF] = 60;}

												 {// WEEK[32
												  WEEK[32].fotoperiodo = 20;          WEEK[32].etapa_cultivo = F("C+R");

												  WEEK[32].temp_sol_nut = 19;

												  WEEK[32].temp_amb[dia] = 28;        WEEK[32].temp_amb[noche] = 21;

												  WEEK[32].hum_amb[dia] = 74;         WEEK[32].hum_amb[noche] = 61;

												  WEEK[32].ph_sol_nut[MIN] = 55;        WEEK[32].ph_sol_nut[MAX] = 65;

												  WEEK[32].ppm_sol_nut[MIN] = 1050;      WEEK[32].ppm_sol_nut[MAX] = 1350;

												  WEEK[32].recirculacion_sol_nut[ON] = 5;   WEEK[32].recirculacion_sol_nut[OFF] = 5;

												  WEEK[32].recambio_aire[ON] = 4;       WEEK[32].recambio_aire[OFF] = 60;}

												 {// WEEK[33
												  WEEK[33].fotoperiodo = 20;          WEEK[33].etapa_cultivo = F("C+R");

												  WEEK[33].temp_sol_nut = 19;

												  WEEK[33].temp_amb[dia] = 28;        WEEK[33].temp_amb[noche] = 21;

												  WEEK[33].hum_amb[dia] = 74;         WEEK[33].hum_amb[noche] = 61;

												  WEEK[33].ph_sol_nut[MIN] = 55;        WEEK[33].ph_sol_nut[MAX] = 65;

												  WEEK[33].ppm_sol_nut[MIN] = 950;       WEEK[33].ppm_sol_nut[MAX] = 1400;

												  WEEK[33].recirculacion_sol_nut[ON] = 5;   WEEK[33].recirculacion_sol_nut[OFF] = 5;

												  WEEK[33].recambio_aire[ON] = 4;       WEEK[33].recambio_aire[OFF] = 60;}

												 {// WEEK[34
												  WEEK[34].fotoperiodo = 12;          WEEK[34].etapa_cultivo = F("RFL");

												  WEEK[34].temp_sol_nut = 19;

												  WEEK[34].temp_amb[dia] = 28;        WEEK[34].temp_amb[noche] = 21;

												  WEEK[34].hum_amb[dia] = 68;         WEEK[34].hum_amb[noche] = 52;

												  WEEK[34].ph_sol_nut[MIN] = 55;        WEEK[34].ph_sol_nut[MAX] = 65;

												  WEEK[34].ppm_sol_nut[MIN] = 900;       WEEK[34].ppm_sol_nut[MAX] = 1300;

												  WEEK[34].recirculacion_sol_nut[ON] = 5;   WEEK[34].recirculacion_sol_nut[OFF] = 5;

												  WEEK[34].recambio_aire[ON] = 4;       WEEK[34].recambio_aire[OFF] = 60;}

												 {// WEEK[35
												  WEEK[35].fotoperiodo = 12;          WEEK[35].etapa_cultivo = F("RFL");

												  WEEK[35].temp_sol_nut = 19;

												  WEEK[35].temp_amb[dia] = 28;        WEEK[35].temp_amb[noche] = 21;

												  WEEK[35].hum_amb[dia] = 68;         WEEK[35].hum_amb[noche] = 52;

												  WEEK[35].ph_sol_nut[MIN] = 55;        WEEK[35].ph_sol_nut[MAX] = 65;

												  WEEK[35].ppm_sol_nut[MIN] = 950;       WEEK[35].ppm_sol_nut[MAX] = 1300;

												  WEEK[35].recirculacion_sol_nut[ON] = 5;   WEEK[35].recirculacion_sol_nut[OFF] = 5;

												  WEEK[35].recambio_aire[ON] = 4;       WEEK[35].recambio_aire[OFF] = 60;}

												 {// WEEK[36
												  WEEK[36].fotoperiodo = 12;          WEEK[36].etapa_cultivo = F("RFL");

												  WEEK[36].temp_sol_nut = 19;

												  WEEK[36].temp_amb[dia] = 28;        WEEK[36].temp_amb[noche] = 21;

												  WEEK[36].hum_amb[dia] = 68;         WEEK[36].hum_amb[noche] = 52;

												  WEEK[36].ph_sol_nut[MIN] = 55;        WEEK[36].ph_sol_nut[MAX] = 65;

												  WEEK[36].ppm_sol_nut[MIN] = 950;       WEEK[36].ppm_sol_nut[MAX] = 1300;

												  WEEK[36].recirculacion_sol_nut[ON] = 5;   WEEK[36].recirculacion_sol_nut[OFF] = 5;

												  WEEK[36].recambio_aire[ON] = 4;       WEEK[36].recambio_aire[OFF] = 60;}

												 {// WEEK[37
												  WEEK[37].fotoperiodo = 12;          WEEK[37].etapa_cultivo = F("RFL");

												  WEEK[37].temp_sol_nut = 19;

												  WEEK[37].temp_amb[dia] = 28;        WEEK[37].temp_amb[noche] = 21;

												  WEEK[37].hum_amb[dia] = 68;         WEEK[37].hum_amb[noche] = 52;

												  WEEK[37].ph_sol_nut[MIN] = 55;        WEEK[37].ph_sol_nut[MAX] = 65;

												  WEEK[37].ppm_sol_nut[MIN] = 1000;      WEEK[37].ppm_sol_nut[MAX] = 1400;

												  WEEK[37].recirculacion_sol_nut[ON] = 5;   WEEK[37].recirculacion_sol_nut[OFF] = 5;

												  WEEK[37].recambio_aire[ON] = 4;       WEEK[37].recambio_aire[OFF] = 60;}

												 {// WEEK[38
												  WEEK[38].fotoperiodo = 12;          WEEK[38].etapa_cultivo = F("RFL");

												  WEEK[38].temp_sol_nut = 19;

												  WEEK[38].temp_amb[dia] = 28;        WEEK[38].temp_amb[noche] = 21;

												  WEEK[38].hum_amb[dia] = 68;         WEEK[38].hum_amb[noche] = 52;

												  WEEK[38].ph_sol_nut[MIN] = 55;        WEEK[38].ph_sol_nut[MAX] = 65;

												  WEEK[38].ppm_sol_nut[MIN] = 1000;      WEEK[38].ppm_sol_nut[MAX] = 1400;

												  WEEK[38].recirculacion_sol_nut[ON] = 5;   WEEK[38].recirculacion_sol_nut[OFF] = 5;

												  WEEK[38].recambio_aire[ON] = 4;       WEEK[38].recambio_aire[OFF] = 60;}

												 {// WEEK[39
												  WEEK[39].fotoperiodo = 12;          WEEK[39].etapa_cultivo = F("RFL");

												  WEEK[39].temp_sol_nut = 19;

												  WEEK[39].temp_amb[dia] = 28;        WEEK[39].temp_amb[noche] = 21;

												  WEEK[39].hum_amb[dia] = 63;         WEEK[39].hum_amb[noche] = 44;

												  WEEK[39].ph_sol_nut[MIN] = 55;        WEEK[39].ph_sol_nut[MAX] = 65;

												  WEEK[39].ppm_sol_nut[MIN] = 1050;      WEEK[39].ppm_sol_nut[MAX] = 1400;

												  WEEK[39].recirculacion_sol_nut[ON] = 5;   WEEK[39].recirculacion_sol_nut[OFF] = 5;

												  WEEK[39].recambio_aire[ON] = 4;       WEEK[39].recambio_aire[OFF] = 60;}

												 {// WEEK[40
												  WEEK[40].fotoperiodo = 12;          WEEK[40].etapa_cultivo = F("RFL");

												  WEEK[40].temp_sol_nut = 19;

												  WEEK[40].temp_amb[dia] = 28;        WEEK[40].temp_amb[noche] = 21;

												  WEEK[40].hum_amb[dia] = 63;         WEEK[40].hum_amb[noche] = 44;

												  WEEK[40].ph_sol_nut[MIN] = 55;        WEEK[40].ph_sol_nut[MAX] = 65;

												  WEEK[40].ppm_sol_nut[MIN] = 1050;      WEEK[40].ppm_sol_nut[MAX] = 1400;

												  WEEK[40].recirculacion_sol_nut[ON] = 5;   WEEK[40].recirculacion_sol_nut[OFF] = 5;

												  WEEK[40].recambio_aire[ON] = 4;       WEEK[40].recambio_aire[OFF] = 60;}

												 {// WEEK[41
												  WEEK[41].fotoperiodo = 12;          WEEK[41].etapa_cultivo = F("RFL");

												  WEEK[41].temp_sol_nut = 19;

												  WEEK[41].temp_amb[dia] = 28;        WEEK[41].temp_amb[noche] = 21;

												  WEEK[41].hum_amb[dia] = 63;         WEEK[41].hum_amb[noche] = 44;

												  WEEK[41].ph_sol_nut[MIN] = 55;        WEEK[41].ph_sol_nut[MAX] = 65;

												  WEEK[41].ppm_sol_nut[MIN] = 1000;      WEEK[41].ppm_sol_nut[MAX] = 1300;

												  WEEK[41].recirculacion_sol_nut[ON] = 5;   WEEK[41].recirculacion_sol_nut[OFF] = 5;

												  WEEK[41].recambio_aire[ON] = 4;       WEEK[41].recambio_aire[OFF] = 60;}

												 {// WEEK[42
												  WEEK[42].fotoperiodo = 12;          WEEK[42].etapa_cultivo = F("RFL");

												  WEEK[42].temp_sol_nut = 19;

												  WEEK[42].temp_amb[dia] = 28;        WEEK[42].temp_amb[noche] = 21;

												  WEEK[42].hum_amb[dia] = 63;         WEEK[42].hum_amb[noche] = 44;

												  WEEK[42].ph_sol_nut[MIN] = 55;        WEEK[42].ph_sol_nut[MAX] = 65;

												  WEEK[42].ppm_sol_nut[MIN] = 750;       WEEK[42].ppm_sol_nut[MAX] = 1150;

												  WEEK[42].recirculacion_sol_nut[ON] = 5;   WEEK[42].recirculacion_sol_nut[OFF] = 5;

												  WEEK[42].recambio_aire[ON] = 4;       WEEK[42].recambio_aire[OFF] = 60;}

												 {// WEEK[43
												  WEEK[43].fotoperiodo = -20;         WEEK[43].etapa_cultivo = F("RFL");

												  WEEK[43].temp_sol_nut = 19;

												  WEEK[43].temp_amb[dia] = 28;        WEEK[43].temp_amb[noche] = 21;

												  WEEK[43].hum_amb[dia] = 63;         WEEK[43].hum_amb[noche] = 44;

												  WEEK[43].ph_sol_nut[MIN] = 55;        WEEK[43].ph_sol_nut[MAX] = 65;

												  WEEK[43].ppm_sol_nut[MIN] = 0;       WEEK[43].ppm_sol_nut[MAX] = 50;

												  WEEK[43].recirculacion_sol_nut[ON] = 5;   WEEK[43].recirculacion_sol_nut[OFF] = 5;

												  WEEK[43].recambio_aire[ON] = 4;       WEEK[43].recambio_aire[OFF] = 60;}

												 {// WEEK[44
												  WEEK[44].fotoperiodo = 12;          WEEK[44].etapa_cultivo = F("RCO");

												  WEEK[44].temp_sol_nut = 19;

												  WEEK[44].temp_amb[dia] = 28;        WEEK[44].temp_amb[noche] = 21;

												  WEEK[44].hum_amb[dia] = 63;         WEEK[44].hum_amb[noche] = 44;

												  WEEK[44].ph_sol_nut[MIN] = 55;        WEEK[44].ph_sol_nut[MAX] = 65;

												  WEEK[44].ppm_sol_nut[MIN] = 0;       WEEK[44].ppm_sol_nut[MAX] = 50;

												  WEEK[44].recirculacion_sol_nut[ON] = 5;   WEEK[44].recirculacion_sol_nut[OFF] = 5;

												  WEEK[44].recambio_aire[ON] = 4;       WEEK[44].recambio_aire[OFF] = 60;}

												 {// WEEK[45
												  WEEK[45].fotoperiodo = -30;         WEEK[45].etapa_cultivo = F("RMA");

												  WEEK[45].temp_sol_nut = -30;

												  WEEK[45].temp_amb[dia] = 22;        WEEK[45].temp_amb[noche] = 22;

												  WEEK[45].hum_amb[dia] = 50;         WEEK[45].hum_amb[noche] = 50;

												  WEEK[45].ph_sol_nut[MIN] = -30;       WEEK[45].ph_sol_nut[MAX] = -30;

												  WEEK[45].ppm_sol_nut[MIN] = -30;       WEEK[45].ppm_sol_nut[MAX] = -30;

												  WEEK[45].recirculacion_sol_nut[ON] = -30; WEEK[45].recirculacion_sol_nut[OFF] = -30;

												  WEEK[45].recambio_aire[ON] = -30;     WEEK[45].recambio_aire[OFF] = -30;}

												 {// WEEK[46
												  WEEK[46].fotoperiodo = -30;         WEEK[46].etapa_cultivo = F("RSE");

												  WEEK[46].temp_sol_nut = -30;

												  WEEK[46].temp_amb[dia] = 22;        WEEK[46].temp_amb[noche] = 22;

												  WEEK[46].hum_amb[dia] = 50;         WEEK[46].hum_amb[noche] = 50;

												  WEEK[46].ph_sol_nut[MIN] = -30;       WEEK[46].ph_sol_nut[MAX] = -30;

												  WEEK[46].ppm_sol_nut[MIN] = -30;       WEEK[46].ppm_sol_nut[MAX] = -30;

												  WEEK[46].recirculacion_sol_nut[ON] = -30; WEEK[46].recirculacion_sol_nut[OFF] = -30;

												  WEEK[46].recambio_aire[ON] = -30;     WEEK[46].recambio_aire[OFF] = -30;}

												 {// WEEK[47
												  WEEK[47].fotoperiodo = -30;         WEEK[47].etapa_cultivo = F("RSE");

												  WEEK[47].temp_sol_nut = -30;

												  WEEK[47].temp_amb[dia] = 22;        WEEK[47].temp_amb[noche] = 22;

												  WEEK[47].hum_amb[dia] = 50;         WEEK[47].hum_amb[noche] = 50;

												  WEEK[47].ph_sol_nut[MIN] = -30;       WEEK[47].ph_sol_nut[MAX] = -30;

												  WEEK[47].ppm_sol_nut[MIN] = -30;       WEEK[47].ppm_sol_nut[MAX] = -30;

												  WEEK[47].recirculacion_sol_nut[ON] = -30; WEEK[47].recirculacion_sol_nut[OFF] = -30;

												  WEEK[47].recambio_aire[ON] = -30;     WEEK[47].recambio_aire[OFF] = -30;}

												 {// WEEK[48
												  WEEK[48].fotoperiodo = -30;         WEEK[48].etapa_cultivo = F("RSE");

												  WEEK[48].temp_sol_nut = -30;

												  WEEK[48].temp_amb[dia] = 22;        WEEK[48].temp_amb[noche] = 22;

												  WEEK[48].hum_amb[dia] = 50;         WEEK[48].hum_amb[noche] = 50;

												  WEEK[48].ph_sol_nut[MIN] = -30;       WEEK[48].ph_sol_nut[MAX] = -30;

												  WEEK[48].ppm_sol_nut[MIN] = -30;       WEEK[48].ppm_sol_nut[MAX] = -30;

												  WEEK[48].recirculacion_sol_nut[ON] = -30; WEEK[48].recirculacion_sol_nut[OFF] = -30;

												  WEEK[48].recambio_aire[ON] = -30;     WEEK[48].recambio_aire[OFF] = -30;}
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- PING --> I2C TODO OK
//			 if(millis() - aux_frecuencia_1 > 2000)
//												{
//												 aux_frecuencia_1 = millis();
//												 I2C_TODO_OK();
//												}
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- LECTURA DE SENSORES

					{//--------------------------------------------------- LECTURA HORA
					 if(millis() - aux_frecuencia_2 > 3600000)
														{
														 aux_frecuencia_2 = millis();

														 I2C_lectura_sensor("hora");

														 hora_actual = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA TEMPERATURA AMBIENTAL 1
					 if(millis() - aux_frecuencia_3 > 2000)
														{
														 aux_frecuencia_3 = millis();

														 I2C_lectura_sensor("tam1");

														 temp_amb_1 = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();
																			   
														 if(temp_amb_1 != 0)
																		{
																		 digitalWrite(A3, LOW);
																		}
														 else if(temp_amb_1 == 0)
																		{
																		 digitalWrite(A3, HIGH);
																		}																			   

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA TEMPERATURA AMBIENTAL 2
					 if(millis() - aux_frecuencia_4 > 2000)
														{
														 aux_frecuencia_4 = millis();

														 I2C_lectura_sensor("tam2");

														 temp_amb_2 = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 if(temp_amb_2 != 0)
																		{
																		 digitalWrite(A3, LOW);
																		}
														 else if(temp_amb_2 == 0)
																		{
																		 digitalWrite(A3, HIGH);
																		}

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA TEMPERATURA AMBIENTAL 3
					 if(millis() - aux_frecuencia_5 > 2000)
														{
														 aux_frecuencia_5 = millis();

														 I2C_lectura_sensor("tam3");

														 temp_amb_3 = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 if(temp_amb_3 != 0)
																		{
																		 digitalWrite(A3, LOW);
																		}
														 else if(temp_amb_3 == 0)
																		{
																		 digitalWrite(A3, HIGH);
																		}
																		
														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA HUMEDAD AMBIENTAL 1
					 if(millis() - aux_frecuencia_7 > 2000)
														{
														 aux_frecuencia_7 = millis();

														 I2C_lectura_sensor("ham1");

														 hum_amb_1 = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA HUMEDAD AMBIENTAL 2
					 if(millis() - aux_frecuencia_8 > 2000)
														{
														 aux_frecuencia_8 = millis();

														 I2C_lectura_sensor("ham2");

														 hum_amb_2 = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA HUMEDAD AMBIENTAL 3
					 if(millis() - aux_frecuencia_9 > 2000)
														{
														 aux_frecuencia_9 = millis();

														 I2C_lectura_sensor("ham3");

														 hum_amb_3 = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			   String(char_mensaje_recibido_del_NodeMcu[6]) +
																			   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA TEMPERATURA SOL. NUT.
					 if(millis() - aux_frecuencia_11 > 2000)
														{
														 aux_frecuencia_11 = millis();

														 SENSOR_TEMP_SOL_NUT.requestTemperatures();
														 temp_sol_nut = SENSOR_TEMP_SOL_NUT.getTempC();

														 aux_temp_sol_nut = 0;														 
														 while(temp_sol_nut > 50 && aux_temp_sol_nut < 10)
																			 {
																			  SENSOR_TEMP_SOL_NUT.requestTemperatures();
																			  temp_sol_nut = SENSOR_TEMP_SOL_NUT.getTempC();
																			  delay(200);
																			  aux_temp_sol_nut = aux_temp_sol_nut + 1;
																			 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA APERTURA PUERTA			 
					 if(millis() - aux_frecuencia_13 > 2000)
														{
														 aux_frecuencia_13 = millis();

														 I2C_lectura_sensor("door");

														 puerta_abierta = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																				  String(char_mensaje_recibido_del_NodeMcu[6]) +
																				  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 if(puerta_abierta == 1)
																			{
																			 aux_puerta_abierta = 0;
																			}
														 if(puerta_abierta == 0 && aux_puerta_abierta < 5)
																			{
																			 aux_puerta_abierta = aux_puerta_abierta + 1;
																			 puerta_abierta = 1;
																			}

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA PPM
					 if(millis() - aux_frecuencia_14 > aux_lectura_EC && CULTIVO_INICIADO == ON)
														{
														 aux_frecuencia_14 = millis();
														 aux_lectura_EC = 1000;
														 
														 INTERRUPTOR("BOMBA_LECTURA_PH_EC", ON);
														 
														 if(aux_inicio_EC_BOMBA_LECTURA_PH_EC == ON)
																								{
																								 aux_frecuencia_39 = millis();
																								 aux_inicio_EC_BOMBA_LECTURA_PH_EC = OFF;
																								}

														 if(millis() - aux_frecuencia_39 > 125000)
																								{
																								 delay(5000);I2C_lectura_sensor("ecsn");

																								 PPM_sol_nut = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																														  String(char_mensaje_recibido_del_NodeMcu[6]) +
																														  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

																								 PPM_sol_nut = (PPM_sol_nut/(1+ (0.0191*(temp_sol_nut - 25))) )*(1/K_EC_sol_nut);

																								 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																			{
																																			 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																			}
																								 INTERRUPTOR("BOMBA_LECTURA_PH_EC", OFF);
																								 aux_inicio_EC_BOMBA_LECTURA_PH_EC = ON;
																								 aux_lectura_EC = 3600000;
																								}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA PH
					//Industrial pH Kit - Atlas scientific
					//https://www.atlas-scientific.com/product_pages/kits/industrial_ph_kit.html
					}
					//____________________________________________________
					{//--------------------------------------------------- BOTON USER FRIENDLY
					 if( digitalRead(pin_boton_lectura_ph) == LOW && 
						 aux_estado_anterior_boton_user_friendly == OFF && 
						 (millis() - aux_tiempo_presionado_boton_user_friendly) > 1000)
											{
											 aux_tiempo_presionado_boton_user_friendly = millis();
											}

					 tiempo_presionado_boton_user_friendly = (millis() - aux_tiempo_presionado_boton_user_friendly);

					 if( digitalRead(pin_boton_lectura_ph) == HIGH &&
						 aux_estado_anterior_boton_user_friendly == ON )
										{
										 if( tiempo_presionado_boton_user_friendly < 10000 &&
											 activar_boton_lectura_ph == ON )
																		 {
																		  boton_lectura_ph_presionado = ON;
																		  activar_boton_lectura_ph = OFF;
																		 }
										 if( tiempo_presionado_boton_user_friendly > 20000 &&
											 activar_boton_RE_FILL_RESERVA_HUMIDIFICADOR == ON )
																		 {
																		  boton_RE_FILL_RESERVA_HUMIDIFICADOR_presionado = ON;
																		  activar_boton_RE_FILL_RESERVA_HUMIDIFICADOR = OFF;
																		 }
										}

					 if(digitalRead(pin_boton_lectura_ph) == HIGH)
																 {
																  aux_estado_anterior_boton_user_friendly = OFF;
																 }
					 if(digitalRead(pin_boton_lectura_ph) == LOW)
																 {
																  aux_estado_anterior_boton_user_friendly = ON;
																 }
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL 2 RECP. SOL. NUT.
					 if(millis() - aux_frecuencia_15 > aux_control_inicio_cultivo)
														{
														 aux_frecuencia_15 = millis();

														 digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, HIGH);

														 if(digitalRead(pin_sensor_nivel_2_RECP_SOL_NUT) == LOW)
																											 {
																											  sensor_nivel_2_RECP_SOL_NUT = ON;
																											  digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, LOW);
																											 }

														 else if (digitalRead(pin_sensor_nivel_2_RECP_SOL_NUT) == HIGH)
																											 {
																											  sensor_nivel_2_RECP_SOL_NUT = OFF;
																											  digitalWrite(pin_5v_sensor_nivel_2_RECP_SOL_NUT, LOW);
																											 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL 1 RECP. SOL. NUT.
					 if(millis() - aux_frecuencia_16 > aux_control_inicio_cultivo)
												{
												 aux_frecuencia_16 = millis();													
													
												 I2C_lectura_sensor("n1sn");

												 sensor_nivel_1_RECP_SOL_NUT = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
																					  String(char_mensaje_recibido_del_NodeMcu[6]) +
																					  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

												 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																							{
																							 char_mensaje_recibido_del_NodeMcu[i] =' ';
																							}
												}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA PPM NIVEL 2 RECP. PPM--
					 if(millis() - aux_frecuencia_17 > aux_control_inicio_cultivo)
														{
														 aux_frecuencia_17 = millis();

														 delay(5000);I2C_lectura_sensor("2ec-");

														 sensor_PPM_nivel_2_RECP_EC_menos = (float)((String(String(char_mensaje_recibido_del_NodeMcu[5]) +
																									  String(char_mensaje_recibido_del_NodeMcu[6]) +
																									  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

														 sensor_PPM_nivel_2_RECP_EC_menos = (sensor_PPM_nivel_2_RECP_EC_menos/(1+ (0.0191*(15 - 25))) )*(1/K_PPM_menos);

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL 1 RECP. PPM--
					 if(millis() - aux_frecuencia_18 > aux_control_inicio_cultivo)
														{
														 aux_frecuencia_18 = millis();

														 I2C_lectura_sensor("1ec-");

														 sensor_nivel_1_RECP_EC_menos = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
																							   String(char_mensaje_recibido_del_NodeMcu[6]) +
																							   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA PPM NIVEL 2 RECP. PPM++
					 if(millis() - aux_frecuencia_19 > aux_control_inicio_cultivo)
														{
														 aux_frecuencia_19 = millis();

														 delay(5000);I2C_lectura_sensor("2ec+");

														 sensor_PPM_nivel_2_RECP_EC_mas = (float)(String(String(char_mensaje_recibido_del_NodeMcu[5]) +
																								   String(char_mensaje_recibido_del_NodeMcu[6]) +
																								   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt());

														 sensor_PPM_nivel_2_RECP_EC_mas = (sensor_PPM_nivel_2_RECP_EC_mas/(1+ (0.0191*(15 - 25))) )*(1/K_PPM_mas);

														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																									{
																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
																									}
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL 1 RECP. PPM++
					 if(millis() - aux_frecuencia_20 > aux_control_inicio_cultivo)
														{
														 aux_frecuencia_20 = millis();

														 digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, HIGH);

														 if(digitalRead(pin_sensor_nivel_1_RECP_EC_mas) == LOW)
																											 {
																											  sensor_nivel_1_RECP_EC_mas = ON;
																											  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, LOW);
																											 }

														 else if (digitalRead(pin_sensor_nivel_1_RECP_EC_mas) == HIGH)
																											 {
																											  sensor_nivel_1_RECP_EC_mas = OFF;
																											  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_mas, LOW);
																											 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL APROX RESERVA HUMIDIFICADOR
					 if( HUMIDIFICADOR == ON )
											{
											 if(millis() - aux_frecuencia_21 > 60000)
																{
																 aux_frecuencia_21 = millis();

																 aux_sensor_nivel_aprox_RECP_HUM = aux_sensor_nivel_aprox_RECP_HUM + 1;
																}

											 if( aux_sensor_nivel_aprox_RECP_HUM <= 300 )
																{
																 sensor_nivel_aprox_RECP_HUM = 9;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 300  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 600 )
																{
																 sensor_nivel_aprox_RECP_HUM = 8;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 600  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 900 )
																{
																 sensor_nivel_aprox_RECP_HUM = 7;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 900  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 1200 )
																{
																 sensor_nivel_aprox_RECP_HUM = 6;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 1200  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 1500 )
																{
																 sensor_nivel_aprox_RECP_HUM = 5;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 1500  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 1800 )
																{
																 sensor_nivel_aprox_RECP_HUM = 4;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 1800  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 2100 )
																{
																 sensor_nivel_aprox_RECP_HUM = 3;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 2100  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 2400 )
																{
																 sensor_nivel_aprox_RECP_HUM = 2;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 2400  && 
												 aux_sensor_nivel_aprox_RECP_HUM <= 2430 )
																{
																 sensor_nivel_aprox_RECP_HUM = 1;
																}
											 if( aux_sensor_nivel_aprox_RECP_HUM > 2700 )
																{
																 sensor_nivel_aprox_RECP_HUM = 0;
																}
											}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL RECP. PH++
//					 if(millis() - aux_frecuencia_22 > aux_control_inicio_cultivo)
//														{
//														 aux_frecuencia_22 = millis();
//
//														 I2C_lectura_sensor("rph+");
//
//														 sensor_nivel_RECP_PH_mas = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
//																							String(char_mensaje_recibido_del_NodeMcu[6]) +
//																							String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();
//
//														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
//																									{
//																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
//																									}
//														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA NIVEL RECP. PH--
//					 if(millis() - aux_frecuencia_23 > aux_control_inicio_cultivo)
//														{
//														 aux_frecuencia_23 = millis();
//
//														 I2C_lectura_sensor("rph-");
//
//														 sensor_nivel_RECP_PH_menos = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
//																							  String(char_mensaje_recibido_del_NodeMcu[6]) +
//																							  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();
//
//														 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
//																									{
//																									 char_mensaje_recibido_del_NodeMcu[i] =' ';
//																									}
//														}
					}
					//____________________________________________________

			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- CONTROL INICIO DE CULTIVO
			 if(CULTIVO_INICIADO == OFF)
										{
										 aux_control_inicio_cultivo = 2000;
											
										 activar_boton_RE_FILL_RESERVA_HUMIDIFICADOR = ON;
										 if(boton_RE_FILL_RESERVA_HUMIDIFICADOR_presionado == ON)
																			{
																			 boton_RE_FILL_RESERVA_HUMIDIFICADOR_presionado = OFF;
																			 
																			 I2C_tocar_cancion(TEMA_FIN_CULTIVO);
																			 aux_sensor_nivel_aprox_RECP_HUM = 0;
																			 sensor_nivel_aprox_RECP_HUM = 9;
																			}

										 if( temp_amb_1 == 0 || temp_amb_2 ==  0 || temp_amb_3 ==  0 ||
											  hum_amb_1 == 0 ||  hum_amb_2 ==  0 ||  hum_amb_3 ==  0 )
																										{
																										 I2C_perifericos("ALRT", "dht");
																										}

										 if( (sensor_nivel_2_RECP_SOL_NUT == ON) &&
											 (sensor_nivel_1_RECP_SOL_NUT == ON) &&
											 (sensor_nivel_aprox_RECP_HUM == 9) && 
											 (CONFIRMACION_INICIO_CULTIVO == ON) )
																			{
																			 I2C_tocar_cancion(TEMA_INICIO_CULTIVO);
																			 
																			 INTERRUPTOR("BOMBA_LECTURA_PH_EC", ON);
																			 delay(125000);
																				
																			 delay(5000);I2C_lectura_sensor("ecsn");

																			 PPM_sol_nut = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																							  String(char_mensaje_recibido_del_NodeMcu[6]) +
																							  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

																			 PPM_sol_nut = (PPM_sol_nut/(1+ (0.0191*(temp_sol_nut - 25))) );


																			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																														{
																														 char_mensaje_recibido_del_NodeMcu[i] =' ';
																														}																	  
																			 INTERRUPTOR("BOMBA_LECTURA_PH_EC", OFF);
																			 
																			 CONFIRMACION_INICIO_CULTIVO = OFF;
																			 CULTIVO_INICIADO = ON;
																			 week_cultivo_actual_ideal = 1;
																			 aux_control_inicio_cultivo = 3600000;
																			}
										}
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- GESTOR DE ETAPAS DE CULTIVO
			 if(CULTIVO_INICIADO == ON)
					{
							{//------------------------------------------- CONTADOR WEEK'S Y DIAS TOTALES
							 if( (millis() - aux_frecuencia_24 > 3600000) && (hora_actual != hora_anterior) )
																			{
																			 aux_frecuencia_24 = millis();
																			 hora_anterior = hora_actual;
																			 
																			 horas_cultivo_total = horas_cultivo_total + 1;
																			 dias_cultivo_total = horas_cultivo_total/24;
																			}
							}
							//____________________________________________
							{//------------------------------------------- CONTADOR DIAS DE CADA ETAPA DE CULTIVO
							 if(dias_cultivo_total != valor_anterior_dias_cultivo_total)
																		{
																		 if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "GER")
																														{dias_cultivo_GERMINACION = dias_cultivo_GERMINACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "PLA")
																														{dias_cultivo_PLANTULA = dias_cultivo_PLANTULA + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "VEG")
																														{dias_cultivo_VEGETACION = dias_cultivo_VEGETACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "FLO")
																														{dias_cultivo_FLORACION = dias_cultivo_FLORACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "COS")
																														{dias_cultivo_COSECHA = dias_cultivo_COSECHA + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "M+R")
																														{dias_cultivo_MANICURA_mas_REVEGETACION = dias_cultivo_MANICURA_mas_REVEGETACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "S+R")
																														{dias_cultivo_SECADO_mas_REVEGETACION = dias_cultivo_SECADO_mas_REVEGETACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "C+R")
																														{dias_cultivo_CURADO_mas_REVEGETACION = dias_cultivo_CURADO_mas_REVEGETACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RFL")
																														{dias_cultivo_RE_FLORACION = dias_cultivo_RE_FLORACION + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RCO")
																														{dias_cultivo_RE_COSECHA = dias_cultivo_RE_COSECHA + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RMA")
																														{dias_cultivo_RE_MANICURA = dias_cultivo_RE_MANICURA + 1;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RSE")
																														{dias_cultivo_RE_SECADO = dias_cultivo_RE_SECADO + 1;}
																		}
							}
							//____________________________________________
							{//------------------------------------------- CONTADOR WEEK'S Y DIAS ACTUAL IDEAL
							 if(dias_cultivo_total != valor_anterior_dias_cultivo_total)
																		{
																		 valor_anterior_dias_cultivo_total = dias_cultivo_total;
																		 
																		 contador_dias_week_cultivo_actual_ideal = contador_dias_week_cultivo_actual_ideal + 1;

																		 if( (contador_dias_week_cultivo_actual_ideal >= 7) &&
																			 (WEEK[week_cultivo_actual_ideal + 1].etapa_cultivo == WEEK[week_cultivo_actual_ideal].etapa_cultivo) )
																												{
																												 week_cultivo_actual_ideal = week_cultivo_actual_ideal + 1;
																												 contador_dias_week_cultivo_actual_ideal = 0;
																												}
																		}
							}
							//____________________________________________
							{//------------------------------------------- CONTROL AVANCE ETAPAS DE CULTIVO
							 if(CONFIRMACION_AVANCE_ETAPA_DE_CULTIVO == ON)
																		{
																		 CONFIRMACION_AVANCE_ETAPA_DE_CULTIVO = OFF;
																		 contador_dias_week_cultivo_actual_ideal = 0;

																		 if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "GER")
																														{week_cultivo_actual_ideal = 2;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "PLA")
																														{week_cultivo_actual_ideal = 4;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "VEG")
																														{week_cultivo_actual_ideal = 16;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "FLO")
																														{week_cultivo_actual_ideal = 26;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "COS")
																														{week_cultivo_actual_ideal = 27;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "M+R")
																														{week_cultivo_actual_ideal = 28;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "S+R")
																														{week_cultivo_actual_ideal = 31;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "C+R")
																														{week_cultivo_actual_ideal = 34;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RFL")
																														{week_cultivo_actual_ideal = 44;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RCO")
																														{week_cultivo_actual_ideal = 45;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RMA")
																														{week_cultivo_actual_ideal = 46;}
																		 else if(WEEK[week_cultivo_actual_ideal].etapa_cultivo == "RSE")
																														{I2C_tocar_cancion(TEMA_FIN_CULTIVO);}
																		}
							}
							//____________________________________________
							{//------------------------------------------- SINCRONIZADOR DE "Parametros Optimos" con "Parametros Week de Cultivo Ideal"
							 if(millis() - aux_frecuencia_25 > 2000)
																{
																 aux_frecuencia_25 = millis();

																 fotoperiodo_optimo_ON = WEEK[week_cultivo_actual_ideal].fotoperiodo;
																 fotoperiodo_optimo_OFF = 24 - fotoperiodo_optimo_ON;
																 temp_sol_nut_optimo = WEEK[week_cultivo_actual_ideal].temp_sol_nut;
																 temp_amb_dia_optimo = WEEK[week_cultivo_actual_ideal].temp_amb[dia];
																 temp_amb_noche_optimo = WEEK[week_cultivo_actual_ideal].temp_amb[noche];
																 hum_amb_dia_optimo = WEEK[week_cultivo_actual_ideal].hum_amb[dia];
																 hum_amb_noche_optimo = WEEK[week_cultivo_actual_ideal].hum_amb[noche];
																 PH_sol_nut_optimo_MIN = WEEK[week_cultivo_actual_ideal].ph_sol_nut[MIN];
																 PH_sol_nut_optimo_MAX = WEEK[week_cultivo_actual_ideal].ph_sol_nut[MAX];
																 PPM_sol_nut_optimo_MIN = (float)(WEEK[week_cultivo_actual_ideal].ppm_sol_nut[MIN]);
																 PPM_sol_nut_optimo_MAX = (float)(WEEK[week_cultivo_actual_ideal].ppm_sol_nut[MAX]);
																 recirculacion_sol_nut_ON_optimo = WEEK[week_cultivo_actual_ideal].recirculacion_sol_nut[ON];
																 recirculacion_sol_nut_OFF_optimo = WEEK[week_cultivo_actual_ideal].recirculacion_sol_nut[OFF];
																 recambio_aire_ON_optimo = WEEK[week_cultivo_actual_ideal].recambio_aire[ON];
																 recambio_aire_OFF_optimo = WEEK[week_cultivo_actual_ideal].recambio_aire[OFF];
																}
							}
							//____________________________________________
					}
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- SINCRONIZADOR EVENTOS ENTRE "Interfaz Grafica" y "Grower"
			 if(millis() - aux_frecuencia_26 > 2000)
						{
							 aux_frecuencia_26 = millis();

							{//------------------------------------------- ENVIO DE DATOS ALOJADOS EN GROWER, LECTURA CONSOLA Y POSTEO DE DATA LOGGER

							 if( (millis() - aux_frecuencia_54 > 900000) || (aux_actualizacion_por_lectura_PH == ON) )
																{
																 I2C_perifericos("nrhu", String( String("00") + String(sensor_nivel_aprox_RECP_HUM) ));

																 if(temp_sol_nut >= 10)
																					 {
																					  I2C_perifericos("tsnu", String( String("0") + String(temp_sol_nut) ));
																					 }
																 if(temp_sol_nut < 10)
																					 {
																					  I2C_perifericos("tsnu", String( String("00") + String(temp_sol_nut) ));
																					 }
																
																 I2C_perifericos("1ec+", String( String("00") + String(sensor_nivel_1_RECP_EC_mas) ));
																
																 if(PH_sol_nut >= 10)
																					 {
																					  I2C_perifericos("phsn", String( String("0") + String(PH_sol_nut) ));
																					 }
																 if(PH_sol_nut < 10)
																					 {
																					  I2C_perifericos("phsn", String( String("00") + String(PH_sol_nut) ));
																					 }													

																 if(dias_cultivo_GERMINACION >= 10)
																					 {
																					  I2C_perifericos("germ", String( String("0") + String(dias_cultivo_GERMINACION) ));
																					 }
																 if(dias_cultivo_GERMINACION < 10)
																					 {
																					  I2C_perifericos("germ", String( String("00") + String(dias_cultivo_GERMINACION) ));
																					 }

																 if(dias_cultivo_PLANTULA >= 10)
																					 {
																					  I2C_perifericos("plnt", String( String("0") + String(dias_cultivo_PLANTULA) ));
																					 }
																 if(dias_cultivo_PLANTULA < 10)
																					 {
																					  I2C_perifericos("plnt", String( String("00") + String(dias_cultivo_PLANTULA) ));
																					 }

																 if(dias_cultivo_VEGETACION >= 100)
																					 {
																					  I2C_perifericos("vege", String(dias_cultivo_VEGETACION) );
																					 }
																 if(dias_cultivo_VEGETACION >= 10 && dias_cultivo_VEGETACION < 100)
																					 {
																					  I2C_perifericos("vege", String( String("0") + String(dias_cultivo_VEGETACION) ));
																					 }
																 if(dias_cultivo_VEGETACION < 10)
																					 {
																					  I2C_perifericos("vege", String( String("00") + String(dias_cultivo_VEGETACION) ));
																					 }

																 if(dias_cultivo_FLORACION >= 100)
																					 {
																					  I2C_perifericos("flor", String(dias_cultivo_FLORACION) );
																					 }
																 if(dias_cultivo_FLORACION >= 10 && dias_cultivo_FLORACION < 100)
																					 {
																					  I2C_perifericos("flor", String( String("0") + String(dias_cultivo_FLORACION) ));
																					 }
																 if(dias_cultivo_FLORACION < 10)
																					 {
																					  I2C_perifericos("flor", String( String("00") + String(dias_cultivo_FLORACION) ));
																					 }

																 if(dias_cultivo_COSECHA >= 10)
																					 {
																					  I2C_perifericos("cose", String( String("0") + String(dias_cultivo_COSECHA) ));
																					 }
																 if(dias_cultivo_COSECHA < 10)
																					 {
																					  I2C_perifericos("cose", String( String("00") + String(dias_cultivo_COSECHA) ));
																					 }

																 if(dias_cultivo_MANICURA_mas_REVEGETACION >= 10)
																					 {
																					  I2C_perifericos("marv", String( String("0") + String(dias_cultivo_MANICURA_mas_REVEGETACION) ));
																					 }
																 if(dias_cultivo_MANICURA_mas_REVEGETACION < 10)
																					 {
																					  I2C_perifericos("marv", String( String("00") + String(dias_cultivo_MANICURA_mas_REVEGETACION) ));
																					 }

																 if(dias_cultivo_SECADO_mas_REVEGETACION >= 10)
																					 {
																					  I2C_perifericos("serv", String( String("0") + String(dias_cultivo_SECADO_mas_REVEGETACION) ));
																					 }
																 if(dias_cultivo_SECADO_mas_REVEGETACION < 10)
																					 {
																					  I2C_perifericos("serv", String( String("00") + String(dias_cultivo_SECADO_mas_REVEGETACION) ));
																					 }

																 if(dias_cultivo_CURADO_mas_REVEGETACION >= 10)
																					 {
																					  I2C_perifericos("curv", String( String("0") + String(dias_cultivo_CURADO_mas_REVEGETACION) ));
																					 }
																 if(dias_cultivo_CURADO_mas_REVEGETACION < 10)
																					 {
																					  I2C_perifericos("curv", String( String("00") + String(dias_cultivo_CURADO_mas_REVEGETACION) ));
																					 }

																 if(dias_cultivo_RE_FLORACION >= 100)
																					 {
																					  I2C_perifericos("refl", String(dias_cultivo_RE_FLORACION) );
																					 }
																 if(dias_cultivo_RE_FLORACION >= 10 && dias_cultivo_RE_FLORACION < 100)
																					 {
																					  I2C_perifericos("refl", String( String("0") + String(dias_cultivo_RE_FLORACION) ));
																					 }
																 if(dias_cultivo_RE_FLORACION < 10)
																					 {
																					  I2C_perifericos("refl", String( String("00") + String(dias_cultivo_RE_FLORACION) ));
																					 }

																 if(dias_cultivo_RE_COSECHA >= 10)
																					 {
																					  I2C_perifericos("reco", String( String("0") + String(dias_cultivo_RE_COSECHA) ));
																					 }
																 if(dias_cultivo_RE_COSECHA < 10)
																					 {
																					  I2C_perifericos("reco", String( String("00") + String(dias_cultivo_RE_COSECHA) ));
																					 }

																 if(dias_cultivo_RE_MANICURA >= 10)
																					 {
																					  I2C_perifericos("rema", String( String("0") + String(dias_cultivo_RE_MANICURA) ));
																					 }
																 if(dias_cultivo_RE_MANICURA < 10)
																					 {
																					  I2C_perifericos("rema", String( String("00") + String(dias_cultivo_RE_MANICURA) ));
																					 }

																 if(dias_cultivo_RE_SECADO >= 10)
																					 {
																					  I2C_perifericos("rese", String( String("0") + String(dias_cultivo_RE_SECADO) ));
																					 }
																 if(dias_cultivo_RE_SECADO < 10)
																					 {
																					  I2C_perifericos("rese", String( String("00") + String(dias_cultivo_RE_SECADO) ));
																					 }

																	{//--------------------------- LECTURA CONSOLA																									  
																	 I2C_lectura_sensor("cnsl");

																	 codigo_leido_desde_consola = String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																										  String(char_mensaje_recibido_del_NodeMcu[6]) +
																										  String(char_mensaje_recibido_del_NodeMcu[7]) );

																	 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																												{
																												 char_mensaje_recibido_del_NodeMcu[i] =' ';
																												}
																	}
																 //__________________________
																 
																 aux_actualizacion_por_lectura_PH = OFF;
																 aux_frecuencia_54 = millis();
																}
							}
							//____________________________________________
							{//------------------------------------------- CONFIRMACION DE INICIO DE CULTIVO
							 if( codigo_leido_desde_consola == "cic" )
																			{
																			 CONFIRMACION_INICIO_CULTIVO = ON;
																			 
																			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																														{
																														 char_mensaje_recibido_del_NodeMcu[i] =' ';
																														}
																			}
							}
							//____________________________________________
							{//------------------------------------------- CAMBIAR VOLUMEN DE RAMAS (BAJO SENSOR 2) (ENTRE SENSOR 1 Y 2)
								
									{//--------------------------- CAMBIO EN VOLUMEN RAMAS BAJO SENSOR 2
									 if( codigo_leido_desde_consola.charAt(0) == 'a' )
																					{
																					 volumen_ramas_bajo_sensor_2 = (codigo_leido_desde_consola.substring(1,3)).toInt();

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'b' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 100 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'c' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 200 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'd' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 300 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'e' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 400 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'f' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 500 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'g' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 600 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'h' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 700 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'i' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 800 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'j' )
																					{
																					 volumen_ramas_bajo_sensor_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 900 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}
									}
									//____________________________________________
									{//--------------------------- CAMBIO EN VOLUMEN RAMAS ENTRE SENSOR 1 Y 2
									 if( codigo_leido_desde_consola.charAt(0) == 'k' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = (codigo_leido_desde_consola.substring(1,3)).toInt();

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'l' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 100 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'm' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 200 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'n' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 300 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'o' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 400 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'p' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 500 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'q' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 600 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 'r' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 700 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 's' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 800 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}

									 if( codigo_leido_desde_consola.charAt(0) == 't' )
																					{
																					 volumen_ramas_entre_sensor_1_y_2 = ( (codigo_leido_desde_consola.substring(1,3)).toInt() + 900 );

																					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																{
																																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																}
																					}
									}
									//____________________________________________
							}
							//____________________________________________
							{//------------------------------------------- CONFIRMACION AVANZE ETAPA DE CULTIVO
							 if( codigo_leido_desde_consola == "cae" )
																			{
																			 CONFIRMACION_AVANCE_ETAPA_DE_CULTIVO = ON;
																			 I2C_tocar_cancion(TEMA_FIN_CULTIVO);

																			 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																														{
																														 char_mensaje_recibido_del_NodeMcu[i] =' ';
																														}
																			}
							}
							//____________________________________________
						 
						 codigo_leido_desde_consola = "";
						}
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- IF :   EL CULTIVO ESTA INICIADO,  ¿ESTA ABIERTA LA PUERTA?
					
					{//---------------------------------------------------------------------- RESPUESTA EN CASO DE PUERTA ABIERTA
					 if( puerta_abierta == ON && CULTIVO_INICIADO == ON )
							{
								
									{//---------------------------------------------- REPRODUCIR PLAYLIST
									 I2C_tocar_cancion(PLAYLIST);
									}
									//_______________________________________________
									{//---------------------------------------------- MANTENER MODO EXPLORACION
									 INTERRUPTOR("BOMBA_RECIRCULACION_SOL_NUT", OFF);
									 INTERRUPTOR("BOMBA_AIRE_OXIGENACION_SOL_NUT", OFF);
									 INTERRUPTOR("BOMBA_LECTURA_PH_EC", OFF);
									 INTERRUPTOR("VALVULA_PH_mas", OFF);
									 INTERRUPTOR("VALVULA_PH_menos", OFF);
									 INTERRUPTOR("TERMOVENTILADOR", OFF);
									 INTERRUPTOR("HUMIDIFICADOR", OFF);
									 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", OFF);
									 INTERRUPTOR("CALEFACTOR_SOL_NUT", OFF);
									 INTERRUPTOR("BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
									 INTERRUPTOR("VENTILADORES_RECAMBIO_AIRE", OFF);
									 INTERRUPTOR("VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
									 INTERRUPTOR("PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
									 INTERRUPTOR("PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
									 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", pwm_OFF);
									 INTERRUPTOR("VENTILADOR_PRINCIPAL", pwm_OFF);
									 INTERRUPTOR("COMPUERTA_AIRE", pwm_OFF);
									}
									//_______________________________________________
									{//---------------------------------------------- USER FRIENDLY LECTURA SENSOR PH
									 activar_boton_lectura_ph = ON;
									 
									 if(boton_lectura_ph_presionado == ON)
																		{
																		 boton_lectura_ph_presionado = OFF;
																		 
																		 I2C_tocar_cancion(TEMA_LECTURA_PH);

																		 INTERRUPTOR("BOMBA_LECTURA_PH_EC", ON);
																		 delay(125000);
																		 
																		 for(int i=0;i<10;i++) 
																							 { 
																							 buf[i]=analogRead(pin_sensor_PH);
																							 delay(100);
																							 }
																		 for( int i = 0 ; i < 9 ; i++ )
																									 {
																									 for( int j = i + 1 ; j < 10 ; j++ )
																																		{
																																		 if( buf[i] > buf[j] )
																																							 {
																																							 temporal=buf[i];
																																							 buf[i]=buf[j];
																																							 buf[j]=temporal;
																																							 }
																																		}
																									 }
																							 
																		 promedio_PH_sol_nut = 0;
																		 promedio_PH_sol_nut = buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]; 
																		 promedio_PH_sol_nut = promedio_PH_sol_nut/6;

																		 PH_sol_nut = (float)((calibracion_ph_M * promedio_PH_sol_nut)*(5.0/1023.0) + calibracion_ph_Y);
																		 
																		 INTERRUPTOR("BOMBA_LECTURA_PH_EC", OFF);
																		 
																		 aux_activar_control_PH = ON;
																		 
																		 I2C_tocar_cancion(PLAYLIST);
																		 aux_actualizacion_por_lectura_PH = ON;
																		}
									}
									//_______________________________________________
									{//---------------------------------------------- USER FRIENDLY RE-FILL RESERVA HUMIDIFICADOR
									 activar_boton_RE_FILL_RESERVA_HUMIDIFICADOR = ON;

									 if(boton_RE_FILL_RESERVA_HUMIDIFICADOR_presionado == ON)
																		{
																		 boton_RE_FILL_RESERVA_HUMIDIFICADOR_presionado = OFF;
																		 
																		 I2C_tocar_cancion(TEMA_FIN_CULTIVO);
																		 aux_sensor_nivel_aprox_RECP_HUM = 0;
																		 sensor_nivel_aprox_RECP_HUM = 9;
																		}
									}
									//_______________________________________________	
									{//---------------------------------------------- ALERTA VIA E-MAIL TIEMPO DE APERTURA PUERTA
									 if(aux_alerta_puerta_abierta == ON)
												{
												 aux_alerta_puerta_abierta = OFF;
												 aux_frecuencia_47 = millis();
												}

									 if(millis() - aux_frecuencia_47 > 1800000)
																		{
																		 aux_alerta_puerta_abierta = ON;
																		 aux_frecuencia_47 = millis();
																		 
																		 I2C_perifericos("ALRT", "pue");
																		}
									}
									//_______________________________________________
							}
					}
					//_______________________________________________________________________
					{//---------------------------------------------------------------------- RESPUESTA EN CASO DE PUERTA CERRADA
					 if( puerta_abierta == OFF && CULTIVO_INICIADO == ON )
							{

									{//---------------------------------------------- SISTEMA ANTI-EMERGENCIA DE CULTIVO

											{//------------------------------ ALERTA VIA E-MAIL ESTADO DE LOS RECIPIENTES
											 if(millis() - aux_frecuencia_28 > 1800000 && PPM_sol_nut_optimo_MAX != -30 && PPM_sol_nut_optimo_MIN != -30)
																				{
																				 if(sensor_PPM_nivel_2_RECP_EC_mas <= PPM_equivalente_nada ||
																					sensor_PPM_nivel_2_RECP_EC_menos <= PPM_equivalente_nada ||
																					sensor_nivel_aprox_RECP_HUM < 3 )
																															{
																															 I2C_perifericos("ALRT", "rci");
																															}
																				}
											}
											//_______________________________
											{//------------------------------ PROTECTOR DESBALANCE SOL. NUT.
											 if(sensor_PPM_nivel_2_RECP_EC_menos <= PPM_equivalente_nada && PPM_sol_nut_optimo_MAX != -30 && PPM_sol_nut_optimo_MIN != -30)
																									{
																									 PROTECTOR_DESBALANCE_SOL_NUT = "STOP_TOTAL";
																									}
																									
											 if(sensor_PPM_nivel_2_RECP_EC_mas <= PPM_equivalente_nada && 
											    sensor_PPM_nivel_2_RECP_EC_menos > PPM_equivalente_nada &&
												 PPM_sol_nut_optimo_MAX != -30 && PPM_sol_nut_optimo_MIN != -30)
																									{
																									 PROTECTOR_DESBALANCE_SOL_NUT = "MODO_SOLO_AGUA";
																									}
																									
											 if(sensor_PPM_nivel_2_RECP_EC_mas > PPM_equivalente_nada && 
											    sensor_PPM_nivel_2_RECP_EC_menos > PPM_equivalente_nada &&
												 PPM_sol_nut_optimo_MAX != -30 && PPM_sol_nut_optimo_MIN != -30 )
																									{
																									 PROTECTOR_DESBALANCE_SOL_NUT = "OFF";
																									}
											}
											//_______________________________
											{//------------------------------ ALERTA VIA E-MAIL DESBALANCE PPM
											 if(millis() - aux_frecuencia_28 > 1800000 && PPM_sol_nut_optimo_MAX != -30 && PPM_sol_nut_optimo_MIN != -30)
																				{
																				 if( PPM_sol_nut >= (PPM_sol_nut_optimo_MAX + holgura_permitida_alerta_desbalance_EC) ||
																				     PPM_sol_nut <= (PPM_sol_nut_optimo_MIN - holgura_permitida_alerta_desbalance_EC)  )
																																		{
																																		 I2C_perifericos("ALRT", "ecs");
																																		}
																				}
											}
											//_______________________________
											{//------------------------------ ALERTA VIA E-MAIL DESBALANCE PH
									/*		 if(millis() - aux_frecuencia_28 > 1800000 && PH_sol_nut_optimo_MAX != -30 && PH_sol_nut_optimo_MIN != -30)
																				{
																				 if( PH_sol_nut >= (PH_sol_nut_optimo_MAX + holgura_permitida_alerta_desbalance_PH) ||
																				     PH_sol_nut <= (PH_sol_nut_optimo_MIN - holgura_permitida_alerta_desbalance_PH)  )
																																		{
																																		 I2C_perifericos("ALRT", "phs");
																																		}
																				}
									*/		}
											//_______________________________
											{//------------------------------ ALERTA VIA E-MAIL DESBALANCE TEMPERATURA AMBIENTAL
											 if(millis() - aux_frecuencia_28 > 1800000)
																				{
																				 if(fotoperiodo_actual == dia)
																											{
																											 if( (promedio_temp_amb >= (temp_amb_dia_optimo + holgura_permitida_alerta_desbalance_temp_amb)) ||
																												 (promedio_temp_amb <= (temp_amb_dia_optimo - holgura_permitida_alerta_desbalance_temp_amb))  )
																																											{
																																											 I2C_perifericos("ALRT", "tam");
																																											}																												 
																											}
																											
																				 if(fotoperiodo_actual == noche)
																											{
																											 if( (promedio_temp_amb >= (temp_amb_noche_optimo + holgura_permitida_alerta_desbalance_temp_amb)) ||
																												 (promedio_temp_amb <= (temp_amb_noche_optimo - holgura_permitida_alerta_desbalance_temp_amb))  )
																																											{
																																											 I2C_perifericos("ALRT", "tam");
																																											}																												 
																											}
																				}
											}
											//_______________________________
											{//------------------------------ ALERTA VIA E-MAIL DESBALANCE HUMEDAD AMBIENTAL
											 if(millis() - aux_frecuencia_28 > 1800000)
																				{
																				 if(fotoperiodo_actual == dia)
																											{
																											 if( (promedio_hum_amb >= (hum_amb_dia_optimo + holgura_permitida_alerta_desbalance_hum_amb)) ||
																												 (promedio_hum_amb <= (hum_amb_dia_optimo - holgura_permitida_alerta_desbalance_hum_amb))  )
																																											{
																																											 I2C_perifericos("ALRT", "hra");
																																											}																												 
																											}
																											
																				 if(fotoperiodo_actual == noche)
																											{
																											 if( (promedio_hum_amb >= (hum_amb_noche_optimo + holgura_permitida_alerta_desbalance_hum_amb)) ||
																												 (promedio_hum_amb <= (hum_amb_noche_optimo - holgura_permitida_alerta_desbalance_hum_amb))  )
																																											{
																																											 I2C_perifericos("ALRT", "hra");
																																											}																												 
																											}
																				}
											}
											//_______________________________
											{//------------------------------ ALERTA VIA E-MAIL DHT22 DESCONECTADO
											 if(millis() - aux_frecuencia_28 > 1800000)
																				{
																				 if( temp_amb_1 ==  0 || temp_amb_2 ==  0 || temp_amb_3 ==  0 ||
																					  hum_amb_1 ==  0 ||  hum_amb_2 ==  0 ||  hum_amb_3 ==  0  )
																																				{
																																				 I2C_perifericos("ALRT", "dht");
																																				}
																				}
											}
											//_______________________________
											{//------------------------------ ALERTA VIA E-MAIL DESBALANCE TEMP SOL. NUT.
											 if(millis() - aux_frecuencia_28 > 1800000 && temp_sol_nut_optimo != -30)
																				{
																				 aux_frecuencia_28 = millis();

																				 if( (temp_sol_nut >= (temp_sol_nut_optimo + holgura_permitida_alerta_desbalance_temp_sol_nut)) ||
																				     (temp_sol_nut <= (temp_sol_nut_optimo - holgura_permitida_alerta_desbalance_temp_sol_nut))  )
																																		{
																																		 I2C_perifericos("ALRT", "tsn");
																																		}
																				}
											}
											aux_frecuencia_28 = millis();
											//_______________________________
									}
									//_______________________________________________
									{//---------------------------------------------- CONTROLES LOGICOS PARA MANTENER PUNTO OPTIMO
									 if(millis() - aux_frecuencia_34 > 2000)
										{
											 aux_frecuencia_34 = millis();
											 
											{//------------------------------ CONTROL DE TEMPERATURA AMBIENTAL
											
											 {//--- anti-error-desconeccion-dht22
											 promedio_temp_amb = (temp_amb_1 + temp_amb_2)/2;
											 
											 if(temp_amb_1 == 0)
																{
																 promedio_temp_amb = temp_amb_2;
																}
											 if(temp_amb_2 == 0)
																{
																 promedio_temp_amb = temp_amb_1;
																}

											 if(temp_amb_1 == 0 && temp_amb_2 == 0)
																{
																 if(fotoperiodo_actual == dia)
																							{	
																							 promedio_temp_amb = temp_amb_dia_optimo;
																							}
																 if(fotoperiodo_actual == noche)
																							{	
																							 promedio_temp_amb = temp_amb_noche_optimo;
																							}
																}
											 }

											 if(fotoperiodo_actual == dia)
																		{
																		 if( promedio_temp_amb >= (temp_amb_dia_optimo + holgura_permitida_control_temp_amb) )
																								{
																								 if( promedio_temp_amb == (temp_amb_dia_optimo + holgura_permitida_control_temp_amb ) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", oooooI_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_dia_optimo + holgura_permitida_control_temp_amb + 1) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", ooooII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_dia_optimo + holgura_permitida_control_temp_amb + 2) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", oooIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_dia_optimo + holgura_permitida_control_temp_amb + 3) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", ooIIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_dia_optimo + holgura_permitida_control_temp_amb + 4) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", oIIIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb > (temp_amb_dia_optimo + holgura_permitida_control_temp_amb + 4) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", IIIIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								}
																																		
																		 if( promedio_temp_amb <= (temp_amb_dia_optimo - holgura_permitida_control_temp_amb) )
																																		{
																																		 INTERRUPTOR("TERMOVENTILADOR", ON);
																																		 INTERRUPTOR("VENTILADOR_PRINCIPAL", pwm_OFF);
																																		}
																																		
																		 if( promedio_temp_amb == temp_amb_dia_optimo )
																																		{
																																		 INTERRUPTOR("VENTILADOR_PRINCIPAL", pwm_OFF);
																																		 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																		}																																													
																		}
																		
											 if(fotoperiodo_actual == noche)
																		{
																		 if( promedio_temp_amb >= (temp_amb_noche_optimo + holgura_permitida_control_temp_amb) )
																								{
																								 if( promedio_temp_amb == (temp_amb_noche_optimo + holgura_permitida_control_temp_amb ) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", oooooI_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_noche_optimo + holgura_permitida_control_temp_amb + 1) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", ooooII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_noche_optimo + holgura_permitida_control_temp_amb + 2) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", oooIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_noche_optimo + holgura_permitida_control_temp_amb + 3) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", ooIIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb == (temp_amb_noche_optimo + holgura_permitida_control_temp_amb + 4) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", oIIIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								 else if( promedio_temp_amb > (temp_amb_noche_optimo + holgura_permitida_control_temp_amb + 4) )
																																							{
																																							 INTERRUPTOR("VENTILADOR_PRINCIPAL", IIIIII_potencia_VENTILADOR_PRINCIPAL);
																																							 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																							}
																								}
																																		
																		 if( promedio_temp_amb <= (temp_amb_noche_optimo - holgura_permitida_control_temp_amb) )
																																		{
																																		 INTERRUPTOR("TERMOVENTILADOR", ON);
																																		 INTERRUPTOR("VENTILADOR_PRINCIPAL", pwm_OFF);
																																		}
																																		
																		 if( promedio_temp_amb == temp_amb_noche_optimo )
																																		{
																																		 INTERRUPTOR("VENTILADOR_PRINCIPAL", pwm_OFF);
																																		 INTERRUPTOR("TERMOVENTILADOR", OFF);
																																		}																													 
																		}
											}
											//_______________________________
											{//------------------------------ CONTROL DE HUMEDAD AMBIENTAL

											 {//--- anti-error-desconeccion-dht22
											 activar_boton_RE_FILL_RESERVA_HUMIDIFICADOR = OFF;

											 promedio_hum_amb = (hum_amb_1 + hum_amb_2 + hum_amb_3)/3;
											 
											 if(hum_amb_1 == 0)
																{
																 promedio_hum_amb = ((hum_amb_2 + hum_amb_3)/ 2);
																}
											 if(hum_amb_2 == 0)
																{
																 promedio_hum_amb = ((hum_amb_1 + hum_amb_3)/ 2);
																}
											 if(hum_amb_3 == 0)
																{
																 promedio_hum_amb = ((hum_amb_1 + hum_amb_2)/ 2);
																}

											 if(hum_amb_1 == 0 && hum_amb_2 == 0)
																{
																 promedio_hum_amb = hum_amb_3;
																}
											 if(hum_amb_1 == 0 && hum_amb_3 == 0)
																{
																 promedio_hum_amb = hum_amb_2;
																}
											 if(hum_amb_2 == 0 && hum_amb_3 == 0)
																{
																 promedio_hum_amb = hum_amb_1;
																}
																
											 if(hum_amb_1 == 0 && hum_amb_2 == 0 && hum_amb_3 == 0)
																{
																 promedio_hum_amb = hum_amb_dia_optimo;
																}
											 }

											 if(fotoperiodo_actual == dia)
																		{
																		 if( promedio_hum_amb >= (hum_amb_dia_optimo + holgura_permitida_control_hum_amb) )
																																		{
																																		 if(aux_ciclo_peltier_1 == ON)
																																									{
																																									 aux_frecuencia_35 = millis();
																																									 aux_ciclo_peltier_1 = OFF;
																																									 aux_ciclo_peltier_2 = ON;
																																									 aux_ciclo_peltier_3 = OFF;
																																									 
																																									 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", ON);
																																									 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", 600);
																																									
																																									 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																									}
																																		
																																		 if(millis() - aux_frecuencia_35 > tiempo_max_modo_frio_deshumidificador && aux_ciclo_peltier_2 == ON)
																																												{
																																												 aux_ciclo_peltier_1 = OFF;
																																												 aux_ciclo_peltier_2 = OFF;
																																												 aux_ciclo_peltier_3 = ON;
																																												 aux_frecuencia_36 = millis();
																																												 
																																												 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", 400);

																																												 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																												}
																																								
																																		 if(aux_ciclo_peltier_3 == ON)
																																									{
																																									 if( millis() - aux_frecuencia_36 > tiempo_max_modo_calor_deshumidificador )
																																																			{
																																																			 aux_ciclo_peltier_1 = ON;
																																																			 aux_ciclo_peltier_2 = OFF;
																																																			 aux_ciclo_peltier_3 = OFF;
																																																			 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", 600);

																																																			 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																																			}
																																									}
																																		}
																																		
																		 if( promedio_hum_amb <= (hum_amb_dia_optimo - holgura_permitida_control_hum_amb) )
																																		{
																																		 INTERRUPTOR("HUMIDIFICADOR", ON);
																																		 
																																		 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", OFF);
																																		 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", pwm_OFF);

																																		 aux_ciclo_peltier_1 = ON;
																																		 aux_ciclo_peltier_2 = OFF;
																																		 aux_ciclo_peltier_3 = OFF;
																																		}
																																		
																		 if( promedio_hum_amb == hum_amb_dia_optimo )
																																		{
																																		 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																		 
																																		 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", OFF);
																																		 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", pwm_OFF);
																																		 
																																		 aux_ciclo_peltier_1 = ON;
																																		 aux_ciclo_peltier_2 = OFF;
																																		 aux_ciclo_peltier_3 = OFF;
																																		}																																													
																		}
																		
											 if(fotoperiodo_actual == noche)
																		{
																		 if( promedio_hum_amb >= (hum_amb_noche_optimo + holgura_permitida_control_hum_amb) )
																																		{
																																		 if(aux_ciclo_peltier_1 == ON)
																																									{
																																									 aux_frecuencia_37 = millis();
																																									 aux_ciclo_peltier_1 = OFF;
																																									 aux_ciclo_peltier_2 = ON;
																																									 aux_ciclo_peltier_3 = OFF;
																																									 
																																									 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", ON);
																																									 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", 600);
																																									
																																									 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																									}
																																		
																																		 if(millis() - aux_frecuencia_37 > tiempo_max_modo_frio_deshumidificador && aux_ciclo_peltier_2 == ON)
																																												{
																																												 aux_ciclo_peltier_1 = OFF;
																																												 aux_ciclo_peltier_2 = OFF;
																																												 aux_ciclo_peltier_3 = ON;
																																												 aux_frecuencia_38 = millis();
																																												 
																																												 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", 400);

																																												 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																												}
																																								
																																		 if(aux_ciclo_peltier_3 == ON)
																																									{
																																									 if( millis() - aux_frecuencia_38 > tiempo_max_modo_calor_deshumidificador )
																																																			{
																																																			 aux_ciclo_peltier_1 = ON;
																																																			 aux_ciclo_peltier_2 = OFF;
																																																			 aux_ciclo_peltier_3 = OFF;
																																																			 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", 600);

																																																			 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																																			}
																																									}
																																		}
																																		
																		 if( promedio_hum_amb <= (hum_amb_noche_optimo - holgura_permitida_control_hum_amb) )
																																		{
																																		 INTERRUPTOR("HUMIDIFICADOR", ON);
																																		 
																																		 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", OFF);
																																		 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", pwm_OFF);

																																		 aux_ciclo_peltier_1 = ON;
																																		 aux_ciclo_peltier_2 = OFF;
																																		 aux_ciclo_peltier_3 = OFF;
																																		}
																																		
																		 if( promedio_hum_amb == hum_amb_noche_optimo )
																																		{
																																		 INTERRUPTOR("HUMIDIFICADOR", OFF);
																																		 
																																		 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", OFF);
																																		 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", pwm_OFF);
																																		 
																																		 aux_ciclo_peltier_1 = ON;
																																		 aux_ciclo_peltier_2 = OFF;
																																		 aux_ciclo_peltier_3 = OFF;
																																		}																																													
																		}
											}
											//_______________________________
											{//------------------------------ CONTROL DE TEMPERATURA SOL. NUT.
											
											if(temp_sol_nut_optimo == -30)
																		{
																		 INTERRUPTOR("BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																		 INTERRUPTOR("VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																		 INTERRUPTOR("PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																		 INTERRUPTOR("PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																		
																		 INTERRUPTOR("CALEFACTOR_SOL_NUT", OFF);
																		}
											 if(temp_sol_nut_optimo != -30)
																		{
																		 if( (temp_sol_nut) >= (temp_sol_nut_optimo + holgura_permitida_control_temp_sol_nut) )
																																		{
																																		 INTERRUPTOR("BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT", ON);
																																		 INTERRUPTOR("VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT", ON);
																																		 INTERRUPTOR("PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT", ON);
																																		 INTERRUPTOR("PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT", ON);
																																		 
																																		 INTERRUPTOR("CALEFACTOR_SOL_NUT", OFF);
																																		}
																																		
																		 if( (temp_sol_nut) <= (temp_sol_nut_optimo - holgura_permitida_control_temp_sol_nut) )
																																		{
																																		 INTERRUPTOR("CALEFACTOR_SOL_NUT", ON);
																																			
																																		 INTERRUPTOR("BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																																		 INTERRUPTOR("VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																																		 INTERRUPTOR("PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																																		 INTERRUPTOR("PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																																		}

																		 if( temp_sol_nut == temp_sol_nut_optimo )
																												{
																												 INTERRUPTOR("BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																												 INTERRUPTOR("VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																												 INTERRUPTOR("PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																												 INTERRUPTOR("PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
																												
																												 INTERRUPTOR("CALEFACTOR_SOL_NUT", OFF);
																												}
																		}
											}
											//_______________________________
											{//------------------------------ CONTROL DE PPM + MANTENER NIVEL ESTABLE SOL. NUT.

											 if(PPM_sol_nut_optimo_MAX != -30 && PPM_sol_nut_optimo_MIN != -30)
																		{
																		 if(millis() - aux_frecuencia_44 > 3600000)
																											{
																											 aux_frecuencia_44 = millis();
																											 
																											 if( sensor_nivel_1_RECP_SOL_NUT == OFF && 
																												 sensor_nivel_2_RECP_SOL_NUT == OFF &&
																												 PROTECTOR_DESBALANCE_SOL_NUT == "OFF" )
																																	{
																																	 volumen_bajo_sensor_1_sol_nut = volumen_bajo_sensor_1_sol_nut - volumen_ramas_entre_sensor_1_y_2 - volumen_ramas_bajo_sensor_2;
																																	 volumen_bajo_sensor_2_sol_nut = volumen_bajo_sensor_2_sol_nut - volumen_ramas_bajo_sensor_2;
																																	 
																																	 V_EC_mas   = (  (PPM_sol_nut*volumen_bajo_sensor_2_sol_nut) - (( (PPM_sol_nut_optimo_MAX + PPM_sol_nut_optimo_MIN)*( porcentaje_EC_ideal_desde_promedio / 200.0 ))*volumen_bajo_sensor_1_sol_nut) + ((volumen_bajo_sensor_1_sol_nut - volumen_bajo_sensor_2_sol_nut)*sensor_PPM_nivel_2_RECP_EC_menos)  ) / (sensor_PPM_nivel_2_RECP_EC_menos - sensor_PPM_nivel_2_RECP_EC_mas);
																																	 V_EC_menos = (volumen_bajo_sensor_1_sol_nut - volumen_bajo_sensor_2_sol_nut) - V_EC_mas;
																																	 
																																	 int_V_EC_mas   = (int)( V_EC_mas );
																																	 int_V_EC_menos   = (int)( V_EC_menos );

																																	 if( (int_V_EC_mas >= 1000) && (int_V_EC_mas < 10000) )
																																												{
																																												 millis_V_EC_mas = String( int_V_EC_mas );
																																												 I2C_perifericos( millis_V_EC_mas , "mef");
																																												}
																																	 if( (int_V_EC_mas >= 100) && (int_V_EC_mas < 1000) )
																																												{
																																												 millis_V_EC_mas = String( String("0") + String(int_V_EC_mas) );
																																												 I2C_perifericos(millis_V_EC_mas, "mef");
																																												}
																																	 if( (int_V_EC_mas >= 10) && (int_V_EC_mas < 100) )
																																												{
																																												 millis_V_EC_mas = String( String("00") + String(int_V_EC_mas) );
																																												 I2C_perifericos(millis_V_EC_mas, "mef");
																																												}
																																	 if( (int_V_EC_mas >= 1) && (int_V_EC_mas < 10) )
																																												{
																																												 millis_V_EC_mas = String( String("000") + String(int_V_EC_mas) );
																																												 I2C_perifericos(millis_V_EC_mas, "mef");
																																												}

																																	 if( (int_V_EC_menos >= 1000) && (int_V_EC_menos < 10000) )
																																												{
																																												 millis_V_EC_menos  = String( int_V_EC_menos );
																																												 I2C_perifericos( millis_V_EC_menos , "mer");
																																												}
																																	 if( (int_V_EC_menos >= 100) && (int_V_EC_menos < 1000) )
																																												{
																																												 millis_V_EC_menos = String( String("0") + String(int_V_EC_menos) );
																																												 I2C_perifericos(millis_V_EC_menos, "mer");
																																												}
																																	 if( (int_V_EC_menos >= 10) && (int_V_EC_menos < 100) )
																																												{
																																												 millis_V_EC_menos = String( String("00") + String(int_V_EC_menos) );
																																												 I2C_perifericos(millis_V_EC_menos, "mer");
																																												}
																																	 if( (int_V_EC_menos >= 1) && (int_V_EC_menos < 10) )
																																												{
																																												 millis_V_EC_menos = String( String("000") + String(int_V_EC_menos) );
																																												 I2C_perifericos(millis_V_EC_menos, "mer");
																																												}
																																	 I2C_perifericos("syEC", "OKK");
																																	}
																																					
																											 if( sensor_nivel_1_RECP_SOL_NUT == OFF && 
																												 sensor_nivel_2_RECP_SOL_NUT == OFF &&
																												 PROTECTOR_DESBALANCE_SOL_NUT == "MODO_SOLO_AGUA" )
																																					{
																																					 I2C_perifericos("syEC", "MSA");
																																					}
																											}
																		}
											}
											//_______________________________
											{//------------------------------ CONTROL DE PH
											 activar_boton_lectura_ph = OFF;

									/*		 if(PH_sol_nut_optimo_MAX == -30 && PH_sol_nut_optimo_MIN == -30)
																			{
																			 INTERRUPTOR("VALVULA_PH_mas", OFF);
																			 INTERRUPTOR("VALVULA_PH_menos", OFF);
																			}
											 if(PH_sol_nut_optimo_MAX != -30 && PH_sol_nut_optimo_MIN != -30)
																		{
																		 if( (millis() - aux_frecuencia_45 > 6*3600000) && aux_activar_control_PH == ON)
																											{
																											 aux_frecuencia_45 = millis();
																											 aux_activar_control_PH = OFF;

																											 if( PH_sol_nut <= PH_sol_nut_optimo_MIN )
																																					{
																																					 INTERRUPTOR("VALVULA_PH_mas", ON);

																																					 delay(millis_dosis_PH_mas);

																																					 INTERRUPTOR("VALVULA_PH_mas", OFF);
																																					}

																											 if( PH_sol_nut >= PH_sol_nut_optimo_MAX )
																																					{
																																					 INTERRUPTOR("VALVULA_PH_menos", ON);

																																					 delay(millis_dosis_PH_menos);

																																					 INTERRUPTOR("VALVULA_PH_menos", OFF);
																																					}
																											}
																		}
									*/		}
											//_______________________________
											{//------------------------------ CONTROL DE PERIODO DE LUZ
											 
											 if(fotoperiodo_optimo_ON == -10)
																			{
																			 if(dias_cultivo_GERMINACION <= 4)
																											{
																											 INTERRUPTOR("LED", OFF);
																											 fotoperiodo_actual = dia;
																											}

																			 if(dias_cultivo_GERMINACION > 4)
																											{
																											 if( aux_ciclo_periodo_luz_1 == ON  && hora_inicio_fotoperiodo_dia == hora_actual )
																																		{
																																		 aux_frecuencia_42 = millis();
																																		 aux_ciclo_periodo_luz_1 = OFF;
																																		 aux_ciclo_periodo_luz_2 = ON;
																																		 aux_ciclo_periodo_luz_3 = OFF;
																																		 
																																		 INTERRUPTOR("LED", ON);
																																		 fotoperiodo_actual = dia;
																																		}
																											
																											 if( millis() - aux_frecuencia_42 > 20*60*60000 && aux_ciclo_periodo_luz_2 == ON )
																																					{
																																					 aux_ciclo_periodo_luz_1 = OFF;
																																					 aux_ciclo_periodo_luz_2 = OFF;
																																					 aux_ciclo_periodo_luz_3 = ON;
																																					 aux_frecuencia_43 = millis();
																																					 
																																					 INTERRUPTOR("LED", OFF);
																																					 fotoperiodo_actual = noche;
																																					}
																																	
																											 if(millis() - aux_frecuencia_43 > 4*60*60000 && aux_ciclo_periodo_luz_3 == ON )
																																					{
																																					 aux_ciclo_periodo_luz_1 = ON;
																																					 aux_ciclo_periodo_luz_2 = OFF;
																																					 aux_ciclo_periodo_luz_3 = OFF;
																																					}
																											}
																			}

											 if(fotoperiodo_optimo_ON == -20)
																			{
																			 if(dias_cultivo_FLORACION == 68 || dias_cultivo_FLORACION == 69 || dias_cultivo_RE_FLORACION == 68 || dias_cultivo_RE_FLORACION == 69 )
																											{
																											 INTERRUPTOR("LED", OFF);
																											 fotoperiodo_actual = noche;
																											}

																			 if(dias_cultivo_FLORACION != 68 && dias_cultivo_FLORACION != 69 && dias_cultivo_RE_FLORACION != 68 && dias_cultivo_RE_FLORACION != 69 )
																											{
																											 if( aux_ciclo_periodo_luz_1 == ON  && hora_inicio_fotoperiodo_dia == hora_actual )
																																		{
																																		 aux_frecuencia_48 = millis();
																																		 aux_ciclo_periodo_luz_1 = OFF;
																																		 aux_ciclo_periodo_luz_2 = ON;
																																		 aux_ciclo_periodo_luz_3 = OFF;
																																		 
																																		 INTERRUPTOR("LED", ON);
																																		 fotoperiodo_actual = dia;
																																		}
																											
																											 if( millis() - aux_frecuencia_48 > 12*60*60000 && aux_ciclo_periodo_luz_2 == ON )
																																					{
																																					 aux_ciclo_periodo_luz_1 = OFF;
																																					 aux_ciclo_periodo_luz_2 = OFF;
																																					 aux_ciclo_periodo_luz_3 = ON;
																																					 aux_frecuencia_49 = millis();
																																					 
																																					 INTERRUPTOR("LED", OFF);
																																					 fotoperiodo_actual = noche;
																																					}
																																	
																											 if(millis() - aux_frecuencia_49 > 12*60*60000 && aux_ciclo_periodo_luz_3 == ON )
																																					{
																																					 aux_ciclo_periodo_luz_1 = ON;
																																					 aux_ciclo_periodo_luz_2 = OFF;
																																					 aux_ciclo_periodo_luz_3 = OFF;
																																					}
																											}
																			}
																			
											 if(fotoperiodo_optimo_ON == -30)
																			{
																			 INTERRUPTOR("LED", OFF);
																			}
																			
											 if( fotoperiodo_optimo_ON != -10 && fotoperiodo_optimo_ON != -20 && fotoperiodo_optimo_ON != -30 )
																			{
																			 if( aux_ciclo_periodo_luz_1 == ON  && hora_inicio_fotoperiodo_dia == hora_actual )
																										{
																										 aux_frecuencia_50 = millis();
																										 aux_ciclo_periodo_luz_1 = OFF;
																										 aux_ciclo_periodo_luz_2 = ON;
																										 aux_ciclo_periodo_luz_3 = OFF;
																										 
																										 INTERRUPTOR("LED", ON);
																										 fotoperiodo_actual = dia;
																										}
																			
																			 if( millis() - aux_frecuencia_50 > fotoperiodo_optimo_ON*60*60000 && aux_ciclo_periodo_luz_2 == ON )
																													{
																													 aux_ciclo_periodo_luz_1 = OFF;
																													 aux_ciclo_periodo_luz_2 = OFF;
																													 aux_ciclo_periodo_luz_3 = ON;
																													 aux_frecuencia_51 = millis();
																													 
																													 INTERRUPTOR("LED", OFF);
																													 fotoperiodo_actual = noche;
																													}
																									
																			 if(millis() - aux_frecuencia_51 > fotoperiodo_optimo_OFF*60*60000 && aux_ciclo_periodo_luz_3 == ON )
																													{
																													 aux_ciclo_periodo_luz_1 = ON;
																													 aux_ciclo_periodo_luz_2 = OFF;
																													 aux_ciclo_periodo_luz_3 = OFF;
																													}
																			}
											}
											//_______________________________
											{//------------------------------ CONTROL DE RECAMBIO DE AIRE

											if(recambio_aire_ON_optimo == -30 && recambio_aire_OFF_optimo == -30)
																			{
																			 INTERRUPTOR("COMPUERTA_AIRE", pwm_OFF);
																			 INTERRUPTOR("VENTILADORES_RECAMBIO_AIRE", OFF);
																			}
											 if(recambio_aire_ON_optimo != -30 && recambio_aire_OFF_optimo != -30 )
																		{
																		 if(aux_ciclo_recambio_aire_1 == ON)
																									{
																									 aux_frecuencia_52 = millis();
																									 aux_ciclo_recambio_aire_1 = OFF;
																									 aux_ciclo_recambio_aire_2 = ON;
																									 aux_ciclo_recambio_aire_3 = OFF;
																									 
																									 INTERRUPTOR("COMPUERTA_AIRE", 600 );
																									 
																									 INTERRUPTOR("VENTILADORES_RECAMBIO_AIRE", ON);
																									}
																		
																		 if(millis() - aux_frecuencia_52 > recambio_aire_ON_optimo*60000 && aux_ciclo_recambio_aire_2 == ON)
																												{
																												 aux_ciclo_recambio_aire_1 = OFF;
																												 aux_ciclo_recambio_aire_2 = OFF;
																												 aux_ciclo_recambio_aire_3 = ON;
																												 aux_frecuencia_53 = millis();
																												 
																												 INTERRUPTOR("COMPUERTA_AIRE", 400 );
																												 INTERRUPTOR("VENTILADORES_RECAMBIO_AIRE", OFF);
																												}
																								
																		 if(millis() - aux_frecuencia_53 > recambio_aire_OFF_optimo*60000 && aux_ciclo_recambio_aire_3 == ON )
																												{
																												 aux_ciclo_recambio_aire_1 = ON;
																												 aux_ciclo_recambio_aire_2 = OFF;
																												 aux_ciclo_recambio_aire_3 = OFF;
																												}
																		}
											}
											//_______________________________
											{//------------------------------ CONTROL DE OXIGENACION SOL. NUT.

											if(temp_sol_nut_optimo == -30)
																			{
																			 INTERRUPTOR("BOMBA_AIRE_OXIGENACION_SOL_NUT", OFF);
																			}
											if(temp_sol_nut_optimo != -30)
																			{
																			 INTERRUPTOR("BOMBA_AIRE_OXIGENACION_SOL_NUT", ON);
																			}
											}
											//_______________________________
											{//------------------------------ CONTROL DE RECIRCULACION SOL. NUT.
											
											if(recirculacion_sol_nut_ON_optimo == -30 && recirculacion_sol_nut_OFF_optimo == -30)
																			{
																			 INTERRUPTOR("BOMBA_RECIRCULACION_SOL_NUT", OFF);
																			}
											 if(recirculacion_sol_nut_ON_optimo != -30 && recirculacion_sol_nut_OFF_optimo != -30)
																		{
																		 if(aux_ciclo_recirculacion_sol_nut_1 == ON)
																									{
																									 aux_frecuencia_40 = millis();
																									 aux_ciclo_recirculacion_sol_nut_1 = OFF;
																									 aux_ciclo_recirculacion_sol_nut_2 = ON;
																									 aux_ciclo_recirculacion_sol_nut_3 = OFF;
																									 
																									 INTERRUPTOR("BOMBA_RECIRCULACION_SOL_NUT", ON);
																									}
																		
																		 if(millis() - aux_frecuencia_40 > recirculacion_sol_nut_ON_optimo*60000 && aux_ciclo_recirculacion_sol_nut_2 == ON)
																												{
																												 aux_ciclo_recirculacion_sol_nut_1 = OFF;
																												 aux_ciclo_recirculacion_sol_nut_2 = OFF;
																												 aux_ciclo_recirculacion_sol_nut_3 = ON;
																												 aux_frecuencia_41 = millis();
																												 
																												 INTERRUPTOR("BOMBA_RECIRCULACION_SOL_NUT", OFF);
																												}
																								
																		 if(millis() - aux_frecuencia_41 > recirculacion_sol_nut_OFF_optimo*60000 && aux_ciclo_recirculacion_sol_nut_3 == ON )
																												{
																												 aux_ciclo_recirculacion_sol_nut_1 = ON;
																												 aux_ciclo_recirculacion_sol_nut_2 = OFF;
																												 aux_ciclo_recirculacion_sol_nut_3 = OFF;
																												}
																		}
											}
											//_______________________________
										}
									}
									//_______________________________________________
							}
					}
					//_______________________________________________________________________
			}
			//_______________________________________________________________________________________			
			{//-------------------------------------------------------------------------------------- GUARDAR BACKUP
			 if(millis() - aux_frecuencia_46 > 2000)
												{
												 aux_frecuencia_46 = millis();
												 SaveData();
												}
			}
			//_______________________________________________________________________________________
			
			
		 {//------------------- iniciar cultivo en una "week_cultivo_actual_ideal" especifica,,, borrar EPRROM de SAF ,,, modo calibracion
		 Serial.println("----------------------------------------------------");
		 Serial.print("fotoperiodo -> 0 = dia , 1 = noche ... -->  ");Serial.println(fotoperiodo_actual);
		 Serial.print("sensor puerta -> 1 = abierta , 0 = cerrada ... -->  ");Serial.println(puerta_abierta);
		 Serial.println("");
		 Serial.print("semana  =  ");Serial.println(week_cultivo_actual_ideal);
		 Serial.print("etapa de cultivo  =  ");Serial.println( WEEK[week_cultivo_actual_ideal].etapa_cultivo );
		 Serial.println("");
		 Serial.print("cultivo iniciado -> 1 = si , 0 = no ... -->  ");Serial.println(CULTIVO_INICIADO);
		 Serial.println("");
					 Serial.println("");
					 Serial.print("temp_amb_1  =  ");Serial.println(temp_amb_1);
					 Serial.print("     temp_amb_2  =  ");Serial.println(temp_amb_2);
					 Serial.print("          temp_amb_3  =  ");Serial.println(temp_amb_3);
					 Serial.println("");
					 Serial.print("hum_amb_1  =  ");Serial.println(hum_amb_1);
					 Serial.print("     hum_amb_2  =  ");Serial.println(hum_amb_2);
					 Serial.print("          hum_amb_3  =  ");Serial.println(hum_amb_3);
					 Serial.println("");
					 Serial.println("");
					 Serial.print("temp_sol_nut  =  ");Serial.println(temp_sol_nut);
					 Serial.println("");
					 Serial.println("");
					 Serial.print("hora en ds3231  =  ");Serial.println(hora_actual);
					 Serial.println("");
					 Serial.println("");
					 Serial.println("");
					 Serial.print("P.P.M. sensor EC solucion_nutritiva  =  ");Serial.println(PPM_sol_nut);
					 Serial.println("");
					 Serial.print("          P.P.M. sensor EC ++  =  ");Serial.println(sensor_PPM_nivel_2_RECP_EC_mas);
					 Serial.println("");
					 Serial.print("                    P.P.M. sensor EC --  =  ");Serial.println(sensor_PPM_nivel_2_RECP_EC_menos);
					 Serial.println("");
					 Serial.print("                         P.H. sensor analogo solucion_nutritiva  =  ");Serial.println(PH_sol_nut);
					 Serial.println("");
		 Serial.println("____________________________________________________");

		 if(Serial.available() > 0)
								{
								 comando_recivido_desde_pc_1 = String(Serial.readString());

								 if( comando_recivido_desde_pc_1 == "BORRAR_EPRROM" )
												{
												 I2C_tocar_cancion(BORRAR_EPRROM);
												 Serial.println("");
												 Serial.println("");
												 Serial.println("EPRROM BORRADA");
												 Serial.println("");
												 Serial.println("");
												}

								 if( ((comando_recivido_desde_pc_1).toInt() >= 1) && ((comando_recivido_desde_pc_1).toInt() <= 48) )
												{
												 week_cultivo_actual_ideal = (comando_recivido_desde_pc_1).toInt();
												 I2C_tocar_cancion(TEMA_FIN_CULTIVO);

												   Serial.println("");
												   Serial.print("semana  =  ");Serial.println((comando_recivido_desde_pc_1).toInt());
												   Serial.print("etapa de cultivo  = ");Serial.println( WEEK[week_cultivo_actual_ideal].etapa_cultivo );
												   Serial.println("");
												 
												 contador_dias_week_cultivo_actual_ideal = 0;
												 
												 INTERRUPTOR("BOMBA_LECTURA_PH_EC", ON);
												 delay(20000);
													
												 delay(5000);I2C_lectura_sensor("ecsn");

												 PPM_sol_nut = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																  String(char_mensaje_recibido_del_NodeMcu[6]) +
																  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

												 Serial.println("");
												 Serial.print("PPM sol nut  =  ");Serial.println(PPM_sol_nut);
												 PPM_sol_nut = (PPM_sol_nut/(1+ (0.0191*(temp_sol_nut - 25))) );

												 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																							{
																							 char_mensaje_recibido_del_NodeMcu[i] =' ';
																							}																	  
												 INTERRUPTOR("BOMBA_LECTURA_PH_EC", OFF);
												 
												 CONFIRMACION_INICIO_CULTIVO = OFF;
												 CULTIVO_INICIADO = ON;
												 fotoperiodo_actual = dia;
												 aux_control_inicio_cultivo = 3600000;
												}

								 while( comando_recivido_desde_pc_1 == "IN_MODO_CALIBRACION" )
												{
												 I2C_tocar_cancion(mp3_no_repeat);

												 { // PING --> I2C TODO OK
												  if(millis() - aux_frecuencia_1 > 2000)
																					{
																					 aux_frecuencia_1 = millis();
																					 I2C_TODO_OK();
																					}
												 }

												 { // Subir lectura de sensores EC a 2 segundos
												  if(aux_calibracion == 0)
																			{
																			 aux_calibracion = 1;
																			 I2C_perifericos("CALI", "999");
																			}
												 }

												 { // todo off, LED ON
												 INTERRUPTOR("LED", ON);
												 INTERRUPTOR("BOMBA_RECIRCULACION_SOL_NUT", OFF);
												 INTERRUPTOR("BOMBA_AIRE_OXIGENACION_SOL_NUT", OFF);
												 INTERRUPTOR("BOMBA_LECTURA_PH_EC", OFF);
												 INTERRUPTOR("VALVULA_PH_mas", OFF);
												 INTERRUPTOR("VALVULA_PH_menos", OFF);
												 INTERRUPTOR("TERMOVENTILADOR", OFF);
												 INTERRUPTOR("HUMIDIFICADOR", OFF);
												 INTERRUPTOR("DISIPADOR_PELTIER_DESHUMIDIFICADOR", OFF);
												 INTERRUPTOR("CALEFACTOR_SOL_NUT", OFF);
												 INTERRUPTOR("BOMBA_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
												 INTERRUPTOR("VENTILADORES_RECAMBIO_AIRE", OFF);
												 INTERRUPTOR("VENTILADORES_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
												 INTERRUPTOR("PELTIER_1_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
												 INTERRUPTOR("PELTIER_2_SISTEMA_ENFRIAMIENTO_SOL_NUT", OFF);
												 INTERRUPTOR("PELTIER_DESHUMIDIFICADOR", pwm_OFF);
												 INTERRUPTOR("VENTILADOR_PRINCIPAL", pwm_OFF);
												 INTERRUPTOR("COMPUERTA_AIRE", pwm_OFF);}

												 {delay(5000);I2C_lectura_sensor("ecsn");
												  PPM_sol_nut = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																			String(char_mensaje_recibido_del_NodeMcu[6]) +
																			String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));
												  PPM_sol_nut = (PPM_sol_nut/(1+ (0.0191*(temp_sol_nut - 25))) )*(1/K_EC_sol_nut);
												  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																							{
																							 char_mensaje_recibido_del_NodeMcu[i] =' ';
																							}}

												 {delay(5000);I2C_lectura_sensor("2ec-");
												  sensor_PPM_nivel_2_RECP_EC_menos = (float)((String(String(char_mensaje_recibido_del_NodeMcu[5]) +
																								String(char_mensaje_recibido_del_NodeMcu[6]) +
																								String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));
												  sensor_PPM_nivel_2_RECP_EC_menos = (sensor_PPM_nivel_2_RECP_EC_menos/(1+ (0.0191*(temp_sol_nut - 25))) )*(1/K_PPM_menos);
												  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																							{
																							 char_mensaje_recibido_del_NodeMcu[i] =' ';
																							}}

												 {delay(5000);I2C_lectura_sensor("2ec+");
												  sensor_PPM_nivel_2_RECP_EC_mas = (float)(String(String(char_mensaje_recibido_del_NodeMcu[5]) +
																							 String(char_mensaje_recibido_del_NodeMcu[6]) +
																							 String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt());
												  sensor_PPM_nivel_2_RECP_EC_mas = (sensor_PPM_nivel_2_RECP_EC_mas/(1+ (0.0191*(temp_sol_nut - 25))) )*(1/K_PPM_mas);
												  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																							{
																							 char_mensaje_recibido_del_NodeMcu[i] =' ';
																							}}

												 Serial.println("----------------------------------------------------");
												 Serial.println("");
												
												 Serial.print("P.P.M. sensor EC solucion_nutritiva  =  ");Serial.println(PPM_sol_nut);
												 Serial.print("                       K_EC_sol_nut  =  ");Serial.println(K_EC_sol_nut);
												 Serial.println("");
												 Serial.println("");
												 Serial.print("          P.P.M. sensor EC ++  =  ");Serial.println(sensor_PPM_nivel_2_RECP_EC_mas);
												 Serial.print("                    K_PPM_mas  =  ");Serial.println(K_PPM_mas);
												 Serial.println("");
												 Serial.println("");
												 Serial.print("                    P.P.M. sensor EC --  =  ");Serial.println(sensor_PPM_nivel_2_RECP_EC_menos);
												 Serial.print("                            K_PPM_menos  =  ");Serial.println(K_PPM_menos);
												 Serial.println("");
												 Serial.println("");
												 Serial.print("                         P.H. sensor analogo solucion_nutritiva  =  ");Serial.println(PH_sol_nut);
												 Serial.print("                                               calibracion_ph_Y  =  ");Serial.println(calibracion_ph_Y);
												 Serial.print("                                               calibracion_ph_M  =  ");Serial.println(calibracion_ph_M);
												 Serial.println("");
												 Serial.println("");
												 Serial.println("");
												 Serial.println("");
												 Serial.print("Temperatura para calibracion sensores EC  =  ");Serial.println(temp_sol_nut);
												 Serial.println("");
												 Serial.println("----->");
												 Serial.println("");
												 Serial.println("");
												 Serial.println("");

												 if(Serial.available() > 0)
																		{
																		 comando_recivido_desde_pc_2 = String(Serial.readString());
																		
																		 if( comando_recivido_desde_pc_2 == "OUT_MODO_CALIBRACION" )
																						{
																						 INTERRUPTOR("LED", OFF);

																						 comando_recivido_desde_pc_1 = "";
																						 comando_recivido_desde_pc_2 = "";
																						 aux_comando_recivido_desde_pc_2 = 0;
																						 aux_1_calibracion_temp_sol_nut = OFF;
																						 PH_sol_nut = -1;
																						 
																						 aux_calibracion = 0;
																						 I2C_perifericos("CALI", "500"); // Bajar lectura de sensores EC a 15 minutos
																						}

																		 if( comando_recivido_desde_pc_2 == "LEER_TEMP_SOL_NUT_MODO_CAL" )
																						{
																						 SENSOR_TEMP_SOL_NUT.requestTemperatures();
																						 temp_sol_nut = SENSOR_TEMP_SOL_NUT.getTempC();
																						 aux_1_calibracion_temp_sol_nut = ON;

																						 aux_temp_sol_nut = 0;														 
																						 while(temp_sol_nut > 50 && aux_temp_sol_nut < 10)
																											 {
																											  SENSOR_TEMP_SOL_NUT.requestTemperatures();
																											  temp_sol_nut = SENSOR_TEMP_SOL_NUT.getTempC();
																											  delay(200);
																											  aux_temp_sol_nut = aux_temp_sol_nut + 1;
																											 }
																						}

																		 if( comando_recivido_desde_pc_2 == "LEER_PH_EN_MODO_CAL" )
																						{
																						 I2C_tocar_cancion(TEMA_LECTURA_PH);

																						 delay(125000);
																						 
																						 for(int i=0;i<10;i++) 
																											 { 
																											 buf[i]=analogRead(pin_sensor_PH);
																											 delay(100);
																											 }
																						 for( int i = 0 ; i < 9 ; i++ )
																													 {
																													 for( int j = i + 1 ; j < 10 ; j++ )
																																						{
																																						 if( buf[i] > buf[j] )
																																											 {
																																											 temporal=buf[i];
																																											 buf[i]=buf[j];
																																											 buf[j]=temporal;
																																											 }
																																						}
																													 }
																											 
																						 promedio_PH_sol_nut = 0;
																						 promedio_PH_sol_nut = buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]; 
																						 promedio_PH_sol_nut = promedio_PH_sol_nut/6;

																						 PH_sol_nut = (float)((calibracion_ph_M * promedio_PH_sol_nut)*(5.0/1023.0) + calibracion_ph_Y);
																						 Serial.print("      PH_sol_nut  =  ");Serial.println(PH_sol_nut);
																						 Serial.println("");
																						 Serial.println("");
																						 Serial.println("      SENSOR P.H. leido");
																						}

																		 if( comando_recivido_desde_pc_2 == "CAL_SENSOR_PPM_sol_nut" && aux_1_calibracion_temp_sol_nut == ON )
																						{
																						  delay(5000);I2C_lectura_sensor("ecsn");

																						  EC_sol_nut = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																														   String(char_mensaje_recibido_del_NodeMcu[6]) +
																														   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

																						  Serial.print("      Valor recibido por i2c -> EC_sol_nut  =  ");Serial.println(EC_sol_nut);
																						  Serial.println("");
																						  Serial.println("");
																						 
																						  EC_sol_nut = ((EC_sol_nut/(1+ (0.0191*(temp_sol_nut - 25))) )/500);
																						 
																						  Serial.print("      K_EC_sol_nut  anterior --> ");Serial.println(K_EC_sol_nut);
																						  Serial.println("");
																						  Serial.println("");

																						  K_EC_sol_nut = EC_sol_nut*(1/valor_sobre_calibracion_EC);

																						  Serial.print("      K_EC_sol_nut  nuevo --> ");Serial.println(K_EC_sol_nut);
																						  Serial.println("");
																						  Serial.println("");

																						  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																	{
																																	 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																	}	

																						  Serial.println("");
																						  Serial.println("");
																						  Serial.println("      SENSOR PPM sol_nut calibrado               PPM leido  =  ");

																						  EC_sol_nut = (EC_sol_nut*500)*(1/K_EC_sol_nut);
																						  Serial.println(EC_sol_nut);
																						  EC_sol_nut = -1;
																						 }

																		 if( comando_recivido_desde_pc_2 == "CAL_SENSOR_PPM_EC_menos" && aux_1_calibracion_temp_sol_nut == ON )
																						{
																						  delay(5000);I2C_lectura_sensor("2ec-");
																						  sensor_EC_nivel_2_RECP_EC_menos = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																																		String(char_mensaje_recibido_del_NodeMcu[6]) +
																																		String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

																						  Serial.print("      Valor recibido por i2c -> sensor_EC_nivel_2_RECP_EC_menos  =  ");Serial.println(sensor_EC_nivel_2_RECP_EC_menos);
																						  Serial.println("");
																						  Serial.println("");

																						  sensor_EC_nivel_2_RECP_EC_menos = ((sensor_EC_nivel_2_RECP_EC_menos/(1+ (0.0191*(temp_sol_nut - 25))) )/500);

																						  Serial.print("      K_EC--  anterior --> ");Serial.println(K_PPM_menos);
																						  Serial.println("");
																						  Serial.println("");

																						  K_PPM_menos = sensor_EC_nivel_2_RECP_EC_menos*(1/valor_sobre_calibracion_EC);

																						  Serial.print("      K_EC--  nuevo--> ");Serial.println(K_PPM_menos);
																						  Serial.println("");
																						  Serial.println("");

																						  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																	{
																																	 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																	}	

																						  Serial.println("");
																						  Serial.println("");
																						  Serial.println("      SENSOR PPM EC-- calibrado               PPM leido  =  ");

																						  sensor_EC_nivel_2_RECP_EC_menos = (sensor_EC_nivel_2_RECP_EC_menos*500)*(1/K_PPM_menos);
																						  Serial.println(sensor_EC_nivel_2_RECP_EC_menos);
																						  sensor_EC_nivel_2_RECP_EC_menos = -1;
																						 }

																		 if( comando_recivido_desde_pc_2 == "CAL_SENSOR_PPM_EC_mas" && aux_1_calibracion_temp_sol_nut == ON )
																						{
																						  delay(5000);I2C_lectura_sensor("2ec+");
																						  sensor_EC_nivel_2_RECP_EC_mas = (float)((String( String(char_mensaje_recibido_del_NodeMcu[5]) +
																																			  String(char_mensaje_recibido_del_NodeMcu[6]) +
																																			  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt()));

																						  Serial.print("      Valor recibido por i2c -> sensor_EC_nivel_2_RECP_EC_mas  =  ");Serial.println(sensor_EC_nivel_2_RECP_EC_mas);
																						  Serial.println("");
																						  Serial.println("");

																						  sensor_EC_nivel_2_RECP_EC_mas = ((sensor_EC_nivel_2_RECP_EC_mas/(1+ (0.0191*(temp_sol_nut - 25))) )/500);

																						  Serial.print("      K_EC++  anterior --> ");Serial.println(K_PPM_mas);
																						  Serial.println("");
																						  Serial.println("");

																						  K_PPM_mas = sensor_EC_nivel_2_RECP_EC_mas*(1/valor_sobre_calibracion_EC);

																						  Serial.print("      K_EC++  nuevo--> ");Serial.println(K_PPM_mas);
																						  Serial.println("");
																						  Serial.println("");

																						  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																	{
																																	 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																	}	

																						  Serial.println("");
																						  Serial.println("");
																						  Serial.print("      SENSOR PPM EC++ calibrado               PPM leido  =  ");

																						  sensor_EC_nivel_2_RECP_EC_mas = (sensor_EC_nivel_2_RECP_EC_mas*500)*(1/K_PPM_mas);
																						  Serial.println(sensor_EC_nivel_2_RECP_EC_mas);
																						  sensor_EC_nivel_2_RECP_EC_mas = -1;
																						 }

																		 if( comando_recivido_desde_pc_2 == "CAL_SENSOR_PPM_sol_nut"  || comando_recivido_desde_pc_2 == "CAL_SENSOR_PPM_EC_menos" || comando_recivido_desde_pc_2 == "CAL_SENSOR_PPM_EC_mas" )
																						{
																						  if( aux_1_calibracion_temp_sol_nut == OFF )
																																 {
																																  Serial.println("AUN NO SE HA SETEADO LA TEMPERATURA PARA CALIBRAR LOS SENSORES DE EC");
																																  Serial.println("");
																																  Serial.println("");
																																 }
																						 }

																		 if( comando_recivido_desde_pc_2 == "CAL_1_2_SENSOR_PH_ValorMenor6" && aux_1_calibracion_sensor_PH == OFF )
																						{
																						 aux_1_calibracion_sensor_PH = ON;

																						 I2C_tocar_cancion(TEMA_LECTURA_PH);

																						 delay(125000);
																						 
																						 for(int i=0;i<10;i++) 
																											 { 
																											 buf[i]=analogRead(pin_sensor_PH);
																											 delay(100);
																											 }
																						 for( int i = 0 ; i < 9 ; i++ )
																													 {
																													 for( int j = i + 1 ; j < 10 ; j++ )
																																						{
																																						 if( buf[i] > buf[j] )
																																											 {
																																											 temporal=buf[i];
																																											 buf[i]=buf[j];
																																											 buf[j]=temporal;
																																											 }
																																						}
																													 }
																											 
																						 aux_calibracion_sensor_PHValorMenor6 = 0;
																						 aux_calibracion_sensor_PHValorMenor6 = buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]; 
																						 aux_calibracion_sensor_PHValorMenor6 = (aux_calibracion_sensor_PHValorMenor6/6.0)*(5.0/1023.0);

																						 Serial.print("      VOLTAJE CAL_1_2_SENSOR_PH_ValorMenor6  =  ");Serial.println(aux_calibracion_sensor_PHValorMenor6);
																						 Serial.println("");
																						 Serial.println("");
																						 Serial.println("");
																						 Serial.println("");
																						 Serial.println("      SENSOR P.H. calibrado al 50%");
																						}

																		 if( comando_recivido_desde_pc_2 == "CAL_2_2_SENSOR_PH_ValorMayor6" && aux_1_calibracion_sensor_PH == ON )
																						{
																						 aux_1_calibracion_sensor_PH = OFF;

																						 I2C_tocar_cancion(TEMA_LECTURA_PH);

																						 delay(125000);
																						 
																						 for(int i=0;i<10;i++) 
																											 { 
																											 buf[i]=analogRead(pin_sensor_PH);
																											 delay(100);
																											 }
																						 for( int i = 0 ; i < 9 ; i++ )
																													 {
																													 for( int j = i + 1 ; j < 10 ; j++ )
																																						{
																																						 if( buf[i] > buf[j] )
																																											 {
																																											 temporal=buf[i];
																																											 buf[i]=buf[j];
																																											 buf[j]=temporal;
																																											 }
																																						}
																													 }
																											 
																						 aux_calibracion_sensor_PHValorMayor6 = 0;
																						 aux_calibracion_sensor_PHValorMayor6 = buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]; 
																						 aux_calibracion_sensor_PHValorMayor6 = (aux_calibracion_sensor_PHValorMayor6/6)*(5.0/1023.0);

																						 Serial.print("      VOLTAJE CAL_2_2_SENSOR_PH_ValorMayor6  =  ");Serial.println(aux_calibracion_sensor_PHValorMayor6);
																						 Serial.println("");
																						 Serial.println("");
																						 Serial.println("");

																						 Serial.print("      calibracion_ph_M  anterior --> ");Serial.println(calibracion_ph_M);
																						 Serial.println("");
																						 Serial.print("      calibracion_ph_Y  anterior --> ");Serial.println(calibracion_ph_Y);
																						 Serial.println("");
																						 Serial.println("");

																						 calibracion_ph_M = (  (valor_sobre_calibracion_PH_ValorMayor6 - valor_sobre_calibracion_PH_ValorMenor6)/(aux_calibracion_sensor_PHValorMayor6 - aux_calibracion_sensor_PHValorMenor6)  );
																						 calibracion_ph_Y = (  valor_sobre_calibracion_PH_ValorMenor6 - calibracion_ph_M*aux_calibracion_sensor_PHValorMenor6  );

																						 Serial.print("      calibracion_ph_M  nuevo --> ");Serial.println(calibracion_ph_M);
																						 Serial.println("");
																						 Serial.print("      calibracion_ph_Y  nuevo --> ");Serial.println(calibracion_ph_Y);
																						 Serial.println("");
																						 Serial.println("");

																						 Serial.println("");
																						 Serial.println("");
																						 Serial.println("      SENSOR P.H. calibrado al 100%               PH leido  =  ");
																						 
																						 PH_sol_nut = (float)((calibracion_ph_M * aux_calibracion_sensor_PHValorMayor6) + calibracion_ph_Y);
																						 Serial.println(PH_sol_nut);
																						}

																		 if( (comando_recivido_desde_pc_2).substring(0,1).toInt() == 1  ) // CAL VALVULA_PH_mas
																						{
																						 aux_comando_recivido_desde_pc_2 = 10*((comando_recivido_desde_pc_2).substring(1).toInt());

																						 INTERRUPTOR("VALVULA_PH_mas", ON);
																						 delay(aux_comando_recivido_desde_pc_2);
																						 INTERRUPTOR("VALVULA_PH_mas", OFF);

																						 Serial.print("VALVULA_PH_mas  -> milisegundos encendida =  ");Serial.println(aux_comando_recivido_desde_pc_2);
																						 Serial.println("");
																						 Serial.println("");
																						}

																		 if( (comando_recivido_desde_pc_2).substring(0,1).toInt() == 2 ) // CAL VALVULA_PH_menos
																						{
																						 aux_comando_recivido_desde_pc_2 = 10*((comando_recivido_desde_pc_2).substring(1).toInt());

																						 INTERRUPTOR("VALVULA_PH_menos", ON);
																						 delay(aux_comando_recivido_desde_pc_2);
																						 INTERRUPTOR("VALVULA_PH_menos", OFF);

																						 Serial.print("VALVULA_PH_menos  -> milisegundos encendida =  ");Serial.println(aux_comando_recivido_desde_pc_2);
																						 Serial.println("");
																						 Serial.println("");
																						}

																		 if( (comando_recivido_desde_pc_2).substring(0,1).toInt() == 3 ) // CAL BOMBA EC++
																						{
																						 aux_comando_recivido_desde_pc_2 = ((comando_recivido_desde_pc_2).substring(1).toInt());

																						 if( (aux_comando_recivido_desde_pc_2 >= 1000) && (aux_comando_recivido_desde_pc_2 < 10000) )
																																	{
																																	 millis_V_EC_mas = String( aux_comando_recivido_desde_pc_2 );
																																	 I2C_perifericos( millis_V_EC_mas , "mef");
																																	}
																						 if( (aux_comando_recivido_desde_pc_2 >= 100) && (aux_comando_recivido_desde_pc_2 < 1000) )
																																	{
																																	 millis_V_EC_mas = String( String("0") + String(aux_comando_recivido_desde_pc_2) );
																																	 I2C_perifericos(millis_V_EC_mas, "mef");
																																	}
																						 if( (aux_comando_recivido_desde_pc_2 >= 10) && (aux_comando_recivido_desde_pc_2 < 100) )
																																	{
																																	 millis_V_EC_mas = String( String("00") + String(aux_comando_recivido_desde_pc_2) );
																																	 I2C_perifericos(millis_V_EC_mas, "mef");
																																	}
																						 if( (aux_comando_recivido_desde_pc_2 >= 1) && (aux_comando_recivido_desde_pc_2 < 10) )
																																	{
																																	 millis_V_EC_mas = String( String("000") + String(aux_comando_recivido_desde_pc_2) );
																																	 I2C_perifericos(millis_V_EC_mas, "mef");
																																	}

																						 millis_V_EC_menos  = String("0000");
																						 I2C_perifericos( millis_V_EC_menos , "mer");

																						 I2C_perifericos("syEC", "OKK");

																						 Serial.print("BOMBA_EC++  -> miliLitros bombeados =  ");Serial.println(aux_comando_recivido_desde_pc_2);
																						 Serial.println("");
																						 Serial.println("");
																						}

																		 if((comando_recivido_desde_pc_2).substring(0,1).toInt() == 4 ) // CAL BOMBA EC--
																						{
																						 aux_comando_recivido_desde_pc_2 = ((comando_recivido_desde_pc_2).substring(1).toInt());

																						 millis_V_EC_mas  = String("0000");
																						 I2C_perifericos( millis_V_EC_mas , "mef");

																						 if( (aux_comando_recivido_desde_pc_2 >= 1000) && (aux_comando_recivido_desde_pc_2 < 10000) )
																																	{
																																	 millis_V_EC_menos = String( aux_comando_recivido_desde_pc_2 );
																																	 I2C_perifericos( millis_V_EC_menos , "mer");
																																	}
																						 if( (aux_comando_recivido_desde_pc_2 >= 100) && (aux_comando_recivido_desde_pc_2 < 1000) )
																																	{
																																	 millis_V_EC_menos = String( String("0") + String(aux_comando_recivido_desde_pc_2) );
																																	 I2C_perifericos(millis_V_EC_menos, "mer");
																																	}
																						 if( (aux_comando_recivido_desde_pc_2 >= 10) && (aux_comando_recivido_desde_pc_2 < 100) )
																																	{
																																	 millis_V_EC_menos = String( String("00") + String(aux_comando_recivido_desde_pc_2) );
																																	 I2C_perifericos(millis_V_EC_menos, "mer");
																																	}
																						 if( (aux_comando_recivido_desde_pc_2 >= 1) && (aux_comando_recivido_desde_pc_2 < 10) )
																																	{
																																	 millis_V_EC_menos = String( String("000") + String(aux_comando_recivido_desde_pc_2) );
																																	 I2C_perifericos(millis_V_EC_menos, "mer");
																																	}

																						 I2C_perifericos("syEC", "OKK");

																						 Serial.print("BOMBA_EC--  -> miliLitros bombeados =  ");Serial.println(10*aux_comando_recivido_desde_pc_2);
																						 Serial.println("");
																						 Serial.println("");
																						}

																		 comando_recivido_desde_pc_2 = "";
																		 aux_comando_recivido_desde_pc_2 = 0;
																		}
												Serial.println("");
												Serial.println("");
												Serial.println("____________________________________________________");
												}

								 comando_recivido_desde_pc_1 = "";
								}}

		}
//########################################################################################################################################################################################################