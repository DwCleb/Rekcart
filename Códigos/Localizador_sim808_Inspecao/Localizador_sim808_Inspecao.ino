  /*
  ###########################################################################################################################################
  # Projeto de localizador usando o Arduino.                                                                                                #
  # Shield SIM808 WARCAR.                                                                                                                   #
  # Ao mandar um SMS com o conteudo 'Local' para o arduino ele responde com um link do GoogleMaps para acesso e visualização da localização.# 
  # By  Cleber Jr - DwCleb                                                                                                                  #  
  # Código com inspeção por Monitor Serial                                                                                                  #              #
  ###########################################################################################################################################
  */
  #include <DFRobot_sim808.h> //inclusão da biblioteca
  #include <SoftwareSerial.h> //inclusão da biblioteca
  
  
  #define PIN_TX  7 // Definindo pino de transmissão serial TX para pino de número 7 - cabo cor azul.
  #define PIN_RX  8 // Definindo pino de transmissão serial RX para pino de número 8 - cabo cor amarela.
  
  SoftwareSerial mySerial(PIN_TX,PIN_RX); // Define os pinos de transmissão serial.
  DFRobot_SIM808 sim808(&mySerial);       // Cenecta RX,TX,PWR.
  
  //+++++ Variaveis GPS +++++
  float   latitude = 0;     //variavel para locação da latitude
  float   longitude = 0;    //variavel para locação da longitude
  bool    started = false;  //variavel para controle de acesso a condicional
  bool    gps = true;       //variavel para acesso a função
  char    server[] = "Localizacao: http://maps.google.com.br/maps?q="; // Array com o sufixo da mensagem a ser enviada, a tratando para um link do GoogleMaps
  String  data;             //variavel de locaçao de dados para tratamento da mensagem
  char    MESSAGE[180];     //variavel de locaçao da mensagem para envio
  
  //+++++ Variaveis para tratamento de mensagem +++++
  #define MESSAGE_LENGTH 160        //variavel definindo tamanho da mensagem
  char    PHONE_NUMBER[16];         //variavel para suportar os digitos do número do telefone.
  char    message[MESSAGE_LENGTH];  //Array definindo tamanho da mensagem, recendo numero definido anteriormente
  int     msg_unread = 0;           //Variavel para controle de condicional recebendo quantidade de mensagens recebidas
  char    phone[16];                //Variavel que aloca o número de telefone que mandou a mensagem
  char    datetime[24];             //Variavel que aloca data e hora da mensagem recebida
  String  msg_recebida;             //Variavel que aloca a mensagem recebida
  String  msg_esperada("Local");    //Variavel usada para comparar a mensagem recebida e executar operações
   
  void setup(){
    mySerial.begin(9600); //Inicializando comunicação serial com SIM808 - Arduino
    Serial.begin(9600);   //Inicializando comunicação serial com PC - Arduino
  
    //+++++ Inicializando o modulo sim808  +++++
    while(!sim808.init()){ 
      delay(1000);       //Pausa de 1 segundo
      Serial.print("\nNão inicializado...\n");
    }
  
    //+++++ Ligando a função GPS da placa  +++++
    if(sim808.attachGPS()){
      Serial.println("\nInicialização do GPS bem sucedida. \n\n Obtento dados...");
      apagar_sms();     //Função para exclusão de mensagens existentes
    }else{ 
      Serial.println("\nFalha...\nEspere alguns minutos.");
      started = false;  //Inabilita a variavel de acesso para condicional
    }
  }
  
  void loop(){
      msg_unread = sim808.isSMSunread(); //Quantidade de mensagens não lidas é alocada na variavel
      Serial.print("Mensagens: ");
      Serial.println(msg_unread);
      if(msg_unread < 1){
        Serial.println("Vazio...\n");
      }else{
        Serial.println("\nMsg recebida!\n\nVerificando solicitação...\n");
        verificar_sms();                 //Função responsavel por verificar conteudo da mensagem
        if(started){
          gps = true;                    //Atribui o valor a variavel para acesso a coleta de dados do GPS
          obter_gps();                   //Função que obtem os dados do gps
          mandar_sms();                  //Função que trata da mensagem e a envia para o número solicitante
        }else{
          Serial.println("Mensagem inválida");
          apagar_sms();                  //Função para apagar as mensagens
          }
        }
  }
  
  void verificar_sms(){
      //+++++ Ler mensagem, trata-a para comparação e a deleta +++++
      if(msg_unread > 0){ 
        sim808.readSMS(msg_unread, message, MESSAGE_LENGTH, phone, datetime); //Ler mensagens e seus dados
        sim808.deleteSMS(msg_unread); //Deleta a mensagem
        Serial.print("\nNumero: ");
        Serial.println(phone);  
        Serial.print("Data e hora: ");
        Serial.println(datetime);        
        Serial.print("Mensagem recebida: ");
        String msg(message);
        Serial.println(msg);
        msg_recebida = msg; //recebe mensagem recebida e joga na variavel
        msg_recebida.trim(); //limpa variavel, eliminando os espaços
        if(msg_recebida == msg_esperada){
          started = true;//Habilita a variavel de acesso para condicional
        }else{
          started = false;//Inabilita a variavel de acesso para condicional
          Serial.print("Tente de novo\n");
        }    
     }    
  }
  
  void mandar_sms(){
    //+++++ Tratando dados da localização +++++
    delay(200);
    data += (server); 
    delay(700);
    data += String(latitude,6);
    delay(700);
    data += ",";
    delay(700);
    data += String(longitude,6);
    delay(700);
    data.toCharArray(MESSAGE,180);
    delay(300);
  
    //+++++ Definindo número de telefone e enviando mensagem com localização +++++
    sim808.sendSMS(phone,MESSAGE);
  
    //+++++ Redefinindo variavel locadora da localização e informando envio +++++
    data = "";        //Atribui o valor em branco na variavel 'data'
    started = false;  //Inabilita a variavel de acesso para condicional
    Serial.print("\nMsg enviada: \n");
    Serial.println(MESSAGE);
    Serial.println("\nMsg enviada c/ sucesso!\n");
    delay(15000);     //Pausa o sistema por 15s
  }
  
  void obter_gps(){
    //+++++ Coleta a latitude e longitude e joga em variaveis +++++
    while(gps){
     if(sim808.getGPS()){
      Serial.print("Latitude: ");
      latitude = sim808.GPSdata.lat;
      Serial.println(latitude,6);
      Serial.print("Longitude: ");
      longitude = sim808.GPSdata.lon;
      Serial.println(longitude,6);
      gps = false;    //Inabilita a variavel de acesso para função
      }
    }
  }
  
  void apagar_sms(){
    Serial.println("Apagando todas SMS posteriores");
    for(int i = 1;i <= 5;i++){      
      mySerial.print("AT+CMGD=\"");//Comunicação com o SIM900 por comando 'AT'
      mySerial.println("1,4\"");   //Comanddo para deletar as mensagens no campo '1', sendo o comando '4' para todas
    }
    Serial.println( "Todas mensagens deletadas..." );  
    delay(800);                     //Pausa o sistema por 0.8s
  }
