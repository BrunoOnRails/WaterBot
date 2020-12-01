//INCLUSAO DA BIBLIOTECA DO SENSOR DE TEMPERATURA E UMIDADE DO AR
#include <dht.h>

#define pinSensorAr 7  // porta analogica sensor do ar

#define intervalo 1000 // DEFINICAO EM MILISEGUNDOS O INTERVALO DA LEITURA

#define rele 6  // definicao do pino do rele

int umidadeMin = 20;
int umidadeMax = 70;

//variavel e instancia do objeto
unsigned long delayIntervalo;
dht sensorDHT;

const int pinoSensor = A5; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO

int analogSoloSeco = 400; //VALOR MEDIDO COM O SOLO SECO 
int analogSoloMolhado = 150; //VALOR MEDIDO COM O SOLO MOLHADO 
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO 
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO
boolean statusValvula = false;

void setup(){
 Serial.begin(9600); //INICIALIZA A SERIAL
 //Serial.println("Fazendo leituras..."); //IMPRIME O TEXTO NO MONITOR SERIAL
 delay(1000); //INTERVALO DE 1 SEGUNDOS
 pinMode(rele, OUTPUT); // declara o pino como saida
 digitalWrite(rele, HIGH);   // define como baixo o status inicial
 

}

String createJsonData(float temperaturaAr, float humidadeAr,float humidadeSolo,boolean statusValvula){
  String temperatura_ar = String(temperaturaAr,2);
  String humidade_ar = String(humidadeAr,2);
  String humidade_solo = String(humidadeSolo,2);
 return "{ \"temperaturaAr\": \""+temperatura_ar+"\", \"humidadeSolo\": \""+humidade_solo+"\", \"humidadeAr\": \""+humidade_ar+"\", \"statusValvula\": \""+statusValvula+"\" }";

}


void loop(){  

  
  
  //inicia a leitura do sensor do ar
 if((millis() - delayIntervalo) > intervalo){
  int chk = sensorDHT.read22(pinSensorAr);

   

  //exibe os dados lidos do sensor de ar
  /* Serial.print("Umidade do ar: ");
   Serial.print(sensorDHT.humidity, 1);
   Serial.println("%");
   Serial.print("Temperratura do ar: ");
   Serial.print(sensorDHT.temperature,1);
   Serial.println("ºC");*/

 
   
   delayIntervalo = millis();
   valorLido = constrain(analogRead(pinoSensor),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
   valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
  /* Serial.print("Umidade do solo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
   Serial.print(valorLido); //IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
   Serial.println("%"); //IMPRIME O CARACTERE NO MONITOR SERIAL*/

    

   if(valorLido < umidadeMin ){  // imprime se a valvula ta aberta
   // Serial.println("Valvula aberta");
    digitalWrite(rele, LOW);
    statusValvula = true;
   }

   else if(valorLido >= umidadeMin && valorLido <= umidadeMax) { // impreme se a valvula ta fechada
   // Serial.println("Valvula fechada");
    digitalWrite(rele, HIGH);
    statusValvula = false;
   }

   else (digitalWrite(rele, HIGH),(statusValvula = false));  // caso a umidade esteja fora dos valores definidos ele deixa a valvula desligada

   //Serial.println("-------------------");
  
   Serial.println(createJsonData(sensorDHT.temperature,sensorDHT.humidity,valorLido,statusValvula));
   
 }

 
 
 
}
