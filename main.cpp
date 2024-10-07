#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <unordered_map>

#include "Logger.h"

#define MAX_BACKLOG 5

// Function to get the MIME type based on the file extension
std::string getMimeType(const std::string& request) {
    // Mapping of common file extensions to MIME types
    static const std::unordered_map<std::string, std::string> mimeTypes = {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".gif", "image/gif"},
        {".svg", "image/svg+xml"},
        {".ico", "image/x-icon"},
        {".txt", "text/plain"},
        {".pdf", "application/pdf"},
        {".zip", "application/zip"},
        // Add more extensions as needed
    };

    // Find the last '.' in the request string to extract the file extension
    std::size_t dotPos = request.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string extension = request.substr(dotPos);
        auto it = mimeTypes.find(extension);
        if (it != mimeTypes.end()) {
            return it->second;  // Return the corresponding MIME type
        }
    }

    // Return a default MIME type if no match is found (for unknown file types)
    return "application/octet-stream";
}

void Respond404(SOCKET clientSocket) {
    // Handle unknown get request and serve 404 page
    std::string body;
    {
        std::stringstream ss;
        std::ifstream in("pages/404.html", std::ios_base::in);
        ss << in.rdbuf();
        body = ss.str();
    }
    // Create 404 HTTP response
    std::string notFoundResponse =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.length()) + "\r\n"
        "Connection: close\r\n\r\n" + body;

    send(clientSocket, notFoundResponse.c_str(), notFoundResponse.length(), 0);
    Logger::fmtLogWarning("Unknown GET request recieved.");
    Logger::fmtLogInfo("404 Not Found Response sent: \n%s", notFoundResponse.c_str());
}

void handleClient(SOCKET clientSocket) {
    char request[1024] = { 0 };
    bool isConnected = true;

    while (isConnected) {
        int iResult = recv(clientSocket, request, sizeof(request), 0);
        if (iResult == SOCKET_ERROR) {
            Logger::fmtLogError("Recieve failed with errror: %ld", WSAGetLastError());
            isConnected = false;
            continue;
        }
        else if (iResult == 0) {
            Logger::fmtLogInfo("Connection closed by client");
            isConnected = false;
            continue;
        }

        Logger::fmtLogInfo("Request content: \n%.*s", iResult, request);
        // Requesting index page
        if (memcmp(request, "GET / ", 6) == 0) {
            std::string body; 
            {
                std::stringstream ss;
                std::ifstream in("pages/index.html", std::ios_base::in);
                ss << in.rdbuf();
                body = ss.str();
            }

            // Create HTTP response
            std::string httpResponse =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n"
                "Connection: close\r\n\r\n" + body;

            // Send response
            if (send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0) == SOCKET_ERROR)
                Logger::fmtLogError("Send failed with error: %ld", WSAGetLastError());

            Logger::fmtLogInfo("Response sent: \n%s", httpResponse.c_str());
        }
        else if (memcmp(request, "GET /", 5) == 0 && std::isalnum(request[5])) {
            std::string requestedFile = "pages/";
            int i = 5;
            while (request[i] != '\0' && request[i] != ' ') {
                requestedFile.push_back(request[i++]);
            }
            
            std::string body;
            {
                std::ifstream infile(requestedFile, std::ios_base::in);
                if (!infile.good()) {
                    Respond404(clientSocket);
                    Logger::fmtLogError("File '%s' not found!", requestedFile.c_str());
                    break;
                }
                std::stringstream ss;
                ss << infile.rdbuf();
                body = ss.str();
            }

            // Find appropriate MIME type
            // Create HTTP response
            std::string httpHeader =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: " + getMimeType(requestedFile) + "\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n"
                "Connection: close\r\n\r\n";

            // Send response
            if (send(clientSocket, httpHeader.c_str(), httpHeader.length(), 0) == SOCKET_ERROR)
                Logger::fmtLogError("Send failed with error: %ld", WSAGetLastError());
            // Send the favicon binary data to the client
            if (send(clientSocket, body.c_str(), body.size(), 0) == SOCKET_ERROR)
                Logger::fmtLogError("Failed to send contents of body with error: %ld", WSAGetLastError());

            Logger::fmtLogInfo("Response sent: \n%s", httpHeader.c_str());
        }
        else {
            Respond404(clientSocket);
        }

        // Close the connection after response is sent
        isConnected = false;
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        Logger::fmtLogError("WSAStartup failed with error: %ld", iResult);
        return 1;
    }

    SOCKET ListenerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenerSocket == INVALID_SOCKET) {
        Logger::fmtLogError("Failed to create socket with error: %ld", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    if (bind(ListenerSocket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        Logger::fmtLogError("Bind failed with error: %ld", WSAGetLastError());
        closesocket(ListenerSocket);
        WSACleanup();
        return 1;
    }

    if (listen(ListenerSocket, MAX_BACKLOG) == SOCKET_ERROR) {
        Logger::fmtLogError("Listen failed with error: %ld", WSAGetLastError());
        closesocket(ListenerSocket);
        WSACleanup();
        return 1;
    }

    Logger::fmtLogInfo("Server is listening on port 8080...");

    while (true) {
        sockaddr_in client = { 0 };
        int clientLength = sizeof(client);

        SOCKET AcceptSocket = accept(ListenerSocket, (sockaddr*)&client, &clientLength);
        if (AcceptSocket == INVALID_SOCKET) {
            Logger::fmtLogError("Failed to accept with error: %ld", WSAGetLastError());
            continue; // Move on to the next connection
        }

        Logger::fmtLogInfo("Client connected...");

        // Handle the client
        handleClient(AcceptSocket);
    }

    closesocket(ListenerSocket);
    WSACleanup();
    return 0;
}
