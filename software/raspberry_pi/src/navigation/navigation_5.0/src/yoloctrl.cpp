#include "yoloctrl.h"

array<int, 4> decToBin(int decimal) {
    array<int, 4> binary = {0, 0, 0, 0};
    for (int i = 3; i >= 0; --i) {
        binary[i] = decimal % 2;
        decimal /= 2;
    }
    
    return binary;
}

// Init class YoloCtrl

YoloCtrl::YoloCtrl(){
    for(auto pin : PINS_STATE_DETECTION){
        gpioSetMode(pin, PI_OUTPUT);
        gpioWrite(pin, 0);  // Inicializa os pinos como LOW
    }

    logMessage("Object YoloCtrl created.");
}

void YoloCtrl::updateData(){
    lock_guard<mutex> lock(mutexIdentifiedObjects);

    try {
        sendStateDetection(); //Definir melhor posição para chamar isto

        boost::asio::io_service io_service;  // io_context substituído por io_service
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 65432));

        tcp::socket socket(io_service);
        acceptor.accept(socket);

        boost::asio::streambuf buffer;
        boost::system::error_code error;
        boost::asio::read_until(socket, buffer, '\n', error);

        if (!error || error == boost::asio::error::eof) {  // EOF tratado corretamente
            istream is(&buffer);
            string received_data;
            getline(is, received_data);
	    // tem que tratar o caso de 
            try {
                json received_json = json::parse(received_data);
                identifiedObjects = process_json(received_json);
            } catch (const json::parse_error& e) {
                logMessage(string("Erro ao interpretar JSON: ") + e.what());
            }
        }
    } catch (exception& e) {
        logMessage(string("Erro: ") + e.what());
    }
}

/*
void YoloCtrl::switchCam(int chooseCam){
    logMessage("Switching camera...");

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) throw ErrorCreatingSocket();
    
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        throw ErrorListening();
        close(sock);
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw ErrorAcceptingConnection();
        close(sock);
        // criar um erro pra isso
    }
    
    int32_t net_number = htonl(chooseCam); 
    send(sock, &net_number, sizeof(net_number), 0);
    close(sock);

    if(cam == chooseCam){
        logMessage(string("Value changed to: ") + (cam == 0 ? "Front camera" : "Bottom camera"));
    } else{
        // gerar erro
    }
}
*/

vector<Object> YoloCtrl::process_json(const json& received_json){
    vector<Object> results;
    try {
        if (received_json.count("data")) {  // 'contains' não existia antes do JSON 3.9.0
            for (const auto& obj : received_json["data"]) {
                if (obj.is_array() && obj.size() >= 6) {
                    Object currentObject;
                    currentObject.topLeftXY[0] = obj[0];
                    currentObject.topLeftXY[1] = obj[1];
                    currentObject.downRightXY[0] = obj[2];
                    currentObject.downRightXY[1] = obj[3];
                    currentObject.confidance = obj[4];
                    currentObject.objectId = obj[5];
                    currentObject.name = "Unknown";

                    if (received_json.count("names") && received_json["names"].count(to_string(currentObject.objectId))) {
                        currentObject.name = received_json["names"][to_string(currentObject.objectId)];
                    }
                    results.emplace_back(currentObject);
                    
                    if(received_json.count("cam")) this->cam = received_json["cam"];
                }
            }
        }
    } catch (const json::exception& e) {
        logMessage(string("Erro ao processar JSON: ") + e.what());
    }
    return results;
}

bool YoloCtrl::foundObject(){
    return !(this->identifiedObjects.empty());
}

array<int, 4> YoloCtrl::getXYXY(string objectName){
    array<int, 4> xyxy = {-1, -1, -1, -1};

    for(const auto& obj : identifiedObjects){
        if(obj.name == objectName){
            xyxy[0] = obj.topLeftXY[0];
            xyxy[1] = obj.topLeftXY[1];
            xyxy[2] = obj.downRightXY[0];
            xyxy[3] = obj.downRightXY[1];
            break;
        }
    }
    return xyxy;
}

// Colocar o objeto de maior confiança aqui e tirar a maquina de estados
string YoloCtrl::greaterConfidanceObject(){
    lock_guard<mutex> lock(mutexIdentifiedObjects);

    if(identifiedObjects.empty()) return "";

    Object confidenceObject = identifiedObjects[0];
    for(const auto& obj : identifiedObjects) if(obj.confidance > confidenceObject.confidance) confidenceObject = obj;

    return confidenceObject.name;
}

bool YoloCtrl::changeStateDetection(int state){
    lock_guard<mutex> lock(mutexIdentifiedObjects); // verificar necessidade
    if(state >= 0 && state <= 15){
        this->stateDetection = state;
        logMessage("State detection changed to: " + to_string(state));
        return true;
    } else {
        logMessage("Invalid state detection value: " + to_string(state));
        return false;
    }
}

void YoloCtrl::sendStateDetection(){
    lock_guard<mutex> lock(mutexIdentifiedObjects); // verificar necessidade
    for(auto value : decToBin(stateDetection)) gpioWrite(PINS_STATE_DETECTION[value], 1);
}

void YoloCtrl::stop(){
    close(new_socket);
    close(server_fd);
}

// End class ToloCtrl