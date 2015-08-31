#include "../../utiles/sockets/sockets.h"
# define CHUNK_SIZE 512

struct sockaddr_in especificarSocketInfo(char* direccion, int puerto) {

	struct sockaddr_in socketInfo;

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = inet_addr(direccion);
	socketInfo.sin_port = htons(puerto);

	return socketInfo;
}

int crearSocket() {
	int yes = 1;
	int fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (fileDescriptor == -1) {	// Compruebo error de mala creacion del fd
		perror("[ERROR] Funcion socket\n");
		exit(-1);
	}
	printf("[OK] Funcion SOCKET: Descriptor creado \n");

	// Hacer que el SO libere el puerto inmediatamente luego de cerrar el socket.
	if (setsockopt(fileDescriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))
			== -1) {
		perror("[ERROR] setsockopt en crear socket");
		exit(1);
	}

	return fileDescriptor;
}

void asociarSocket(int sockfd, int puerto) {// Seteamos los valores a la estructura miDireccion
	struct sockaddr_in miDireccion;
	miDireccion.sin_family = AF_INET;
	miDireccion.sin_port = htons(puerto);
	miDireccion.sin_addr.s_addr = 0; // htonl(INADDR_ANY); // Usa mi direccion IP
	memset(&(miDireccion.sin_zero), '\0', 8);

	// Si el puerto esta en uso, lanzamos error
	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse,
			sizeof(int)) == -1) {
		perror("[ERROR] No es posible reusar el socket\n");
		exit(1);
	}

	// Funcion bind
	int posibleError = bind(sockfd, (struct sockaddr *) &miDireccion,
			sizeof(struct sockaddr));

	if (posibleError == -1) {
		perror("[ERROR] Funcion BIND: No se pudo asociar con el puerto\n");
		exit(-1);
	}

	printf("[OK] Funcion BIND. Vinculada al puerto [ %d ]\n", puerto);
}

int conectarSocket(int sockfd, char* ip_Destino, int puerto) { // Seteo estructura datosServidor
	struct sockaddr_in datosServidor;
	datosServidor.sin_family = AF_INET;
	datosServidor.sin_port = htons(puerto);
	datosServidor.sin_addr.s_addr = inet_addr(ip_Destino);
	memset(&(datosServidor.sin_zero), '\0', 8);

	int posibleError = connect(sockfd, (struct sockaddr *) &datosServidor,
			sizeof(struct sockaddr));
	if (posibleError == -1) {
		perror("[ERROR] Funcion connect\n");
		return (-1);
	}
	printf("[OK] Funcion CONNECT\n");
	return (1);
}

int conectarSocketPorPrimeraVez(int sockfd, char* ip_Destino, int puerto) {	// Seteo estructura datosServidor
	struct sockaddr_in datosServidor;
	datosServidor.sin_family = AF_INET;
	datosServidor.sin_port = htons(puerto);
	datosServidor.sin_addr.s_addr = inet_addr(ip_Destino);
	memset(&(datosServidor.sin_zero), '\0', 8);

	int pudoConectarse = -1;
	while (pudoConectarse < 0) {
		pudoConectarse = connect(sockfd, (struct sockaddr *) &datosServidor,
				sizeof(struct sockaddr));
		if (pudoConectarse == -1) {
			printf("Esperando a que se levante el servidor para conectarse\n");
		}
	}
	printf("[OK] Funcion CONNECT\n");
	return (1);
}

void escucharSocket(int sockfd, int conexionesEntrantesPermitidas) {
	int posibleError = listen(sockfd, conexionesEntrantesPermitidas);
	if (posibleError == -1) {
		perror("[ERROR] Funcion listen\n");
		exit(-1);
	}
	printf("[OK] Funcion LISTEN. Admite [ %d ] conexiones entrantes\n",
			conexionesEntrantesPermitidas);
}

int aceptarConexionSocket(int sockfd) {
	struct sockaddr_storage cliente;
	unsigned int addres_size = sizeof(cliente);

	int otroFD = accept(sockfd, (struct sockaddr*) &cliente, &addres_size);

	if (otroFD == -1) {
		perror("[ERROR] Funcion accept");
		exit(-1);
	}
	printf("[OK] Funcion ACCEPT\n");

	return otroFD;
}
// envia por socket toda la info, porque a veces el SO manda mas o menos , por eso el ciclo para asegurarnos que mande todo
int enviarPorSocket(int fdCliente, const void *msg, int *len) {

	int bytesleft = *len; // cuántos se han quedado pendientes
	int bytes_enviados; //n
	int total = 0;

	while (total < *len) {
		bytes_enviados = send(fdCliente, msg + total, bytesleft, 0);
		if (bytes_enviados == -1) {
			break;
		}
		total += bytes_enviados;
		bytesleft -= bytes_enviados;
	}
	*len = total; // devuelve aquí la cantidad enviada en realidad

	if (bytes_enviados == -1) {
		perror("[ERROR] Funcion send\bytes_enviados");
		exit(-1);
	}

	return bytes_enviados;
}

int recibirPorSocket(int fdCliente, void *buf, int len) {
	int bytes_recibidos = recv(fdCliente, buf, len, 0);

	if (bytes_recibidos == -1) {
		perror("[ERROR] Funcion recv\n");
		exit(-1);
	}

	/*if(bytes_recibidos == 0){
	 **		printf("File descriptor desconectado\n");
	 **}
	 */
	return bytes_recibidos;
}

void cerrarSocket(int sockfd) {	// El unico fin de tener esta funcion espara ni hacer el include unistd.h
	close(sockfd);
}

void seleccionarSocket(int maxNumDeFD, fd_set *fdListoLectura,
		fd_set *fdListoEscritura, fd_set *fdListoEjecucion, int* segundos,
		int* milisegundos) {
	struct timeval tv;
	// Esta estructura de tiempo de permite establecer un período máximo de espera.
	// ~ Si segundos = &0 y miliSegundos = &0 => regresará inmediatamente después de interrogar
	//   a todos tus file descriptor
	// ~ Si segundos = NULL y miliSegundos = NULL => espera infinitamente

	int sec = *segundos;
	int mic = (*milisegundos) * 1000;
	tv.tv_sec = sec;
	tv.tv_usec = mic;

	int posibleError = select((maxNumDeFD + 1), fdListoLectura,
			fdListoEscritura, fdListoEjecucion, &tv);

	if (posibleError == -1) {
		perror("[ERROR] Funcion select\n");
		exit(-1);
	}
}

//Recibe un mensaje del servidor - Version Lucas
int recibirMensaje(int unSocket, void** buffer) {

	Header header;
	int auxInt;
	if ((auxInt = recv(unSocket, &header, sizeof(Header), 0)) >= 0) {
		*buffer = malloc(header.payloadTamanio);
		if ((auxInt = recv(unSocket, *buffer, header.payloadTamanio, 0)) >= 0) {
			return auxInt;
		}
	}
	return auxInt;

}
//Recibe un mensaje del servidor: el header - Version Lucas // revisar bien esta funcion
int recibirHeader(int unSocket, Header* header) {
	int auxInt;
	if ((auxInt = recv(unSocket, header, sizeof(Header), 0)) >= 0) {
		return auxInt;
	}
	return auxInt;
}
// recibe el tamanio del payload
int recibirDatos(int unSocket, Header header, void** buffer) {
	int auxInt;
	*buffer = malloc(header.payloadTamanio);
	if ((auxInt = recv(unSocket, buffer, header.payloadTamanio, 0)) >= 0) {
		return auxInt;
	}
	return auxInt;
}

//Mande un mensaje a un socket determinado
int mandarMensaje(int unSocket, int8_t tipo, int tamanio, void *buffer) {

	Header header;
	int auxInt;
	//Que el tamanio lo mande
	void* bufferAux;

	header.tipoMensaje = tipo;
	header.payloadTamanio = tamanio;
	bufferAux = malloc(sizeof(Header) + tamanio);

	memcpy(bufferAux, &header, sizeof(Header));
	memcpy((bufferAux + (sizeof(Header))), buffer, tamanio);
//			if ((auxInt=send(unSocket, &header, sizeof(Header), 0)) >= 0){
	auxInt = send(unSocket, bufferAux, (sizeof(Header) + tamanio), 0);
	free(bufferAux);
	return auxInt;

//			}

}

