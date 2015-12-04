#include "sockets.h"
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
			if(errno==EINTR){
				continue;
			}
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

int enviar(int fdCliente, void *msg, int len) {

	return enviarSimple(fdCliente, msg, len);
}

int enviarStruct(int fdCliente, t_tipo_mensaje tipoMensaje, void *estructura) {

	//serializamos la estructura
	if(existeSerializacion(tipoMensaje)) {
		return enviarSerializado(fdCliente, tipoMensaje, estructura);
	} else {
		printf("No existe serializacion para el tipoMensaje %s\n", getNombreTipoMensaje(tipoMensaje));
		exit(-1);
		return 0;
//		char* estructuraSerializada = serializarEstructura(tipoMensaje, estructura);
//		int longitudMensajeSerializado = strlen(estructuraSerializada);
//
//		//enviamos un header
//		enviarHeader(fdCliente, tipoMensaje, estructura, longitudMensajeSerializado);
//
//		//enviamos la estructura serializada
//		return enviarSimple(fdCliente, estructura, longitudMensajeSerializado);
	}
}

int recibirStructSegunHeader(int fdCliente, t_header* header, void* buffer, t_resultado_serializacion* resultadoSerializacion) {

	//recibimos los datos segun el header

	//deserializamos la estructura
	if(existeSerializacion(header->tipoMensaje)) {
		return recibirSerializado(fdCliente, *header, buffer, resultadoSerializacion);
	} else {
		void* bufferMsgSerializado = malloc(header->tamanioMensaje);
		int res = recibirPorSocket(fdCliente, &bufferMsgSerializado, header->tamanioMensaje);
		if(res > 0){
			//deserializamos la estructura
			deserializarMensajeABuffer(header->tipoMensaje, bufferMsgSerializado, header->tamanioMensaje, buffer);
		}
		return res;
	}
}

char* serializarEstructura(t_tipo_mensaje tipoMensaje, void* bufferMensaje) {

	char* serializado = NULL;



	if(HEADER == tipoMensaje) {
		char* HEADER_FORMAT = "tipoMensaje: %d, tamanioMensaje %d";
		serializado = string_new();
		t_header* header = malloc(sizeof(t_header));
		memcpy(header, bufferMensaje, sizeof(t_header));
		string_append_with_format(&serializado, HEADER_FORMAT, header->tipoMensaje, header->tamanioMensaje);
	} else if(STRING == tipoMensaje) {
		serializado = bufferMensaje;
	}

	return serializado;
}

int deserializarMensajeABuffer(t_tipo_mensaje tipoMensaje, char* bufferMsgSerializado, int tamanioMensaje, void* buffer) {

	if(HEADER == tipoMensaje) {
//		serializado = string_new();
//		t_header* header = malloc(sizeof(t_header));
//		memcpy(header, bufferMensaje, sizeof(t_header));
//		string_append_with_format(&serializado, HEADER_FORMAT, header->tipoMensaje, header->tamanioMensaje);
		char* HEADER_FORMAT = "tipoMensaje: %d, tamanioMensaje %d";
		t_header* header;
		t_tipo_mensaje tipoMensajeLeido;
		int tamanioMensaje = -1;
		sscanf(bufferMsgSerializado, HEADER_FORMAT, &tipoMensajeLeido, tamanioMensaje);
		header->tamanioMensaje = tamanioMensaje;
		header->tipoMensaje = tipoMensajeLeido;
		memcpy(buffer, &header, sizeof(t_header));
	} else if(STRING == tipoMensaje) {
//		serializado = bufferMensaje;
		memcpy(buffer, bufferMsgSerializado, tamanioMensaje);
	}
	return 0;
}

bool string_equals(char* string1, char* string2) {
	return strcmp(string1, string2) == 0;
}

int enviarHeader(int fdCliente, t_header header) {
	return enviarSimple(fdCliente, &header, sizeof(t_header));
}

int contadorMensajes = 0;
t_header crearHeader(t_tipo_mensaje tipoMensaje, void *msg, int longitudMensaje) {
	t_header* header = malloc(sizeof(t_header));

	header->tipoMensaje = tipoMensaje;
	header->tamanioMensaje = longitudMensaje;
	header->numeroMensaje = contadorMensajes++;
	header->nombre = getNombre();

	return *header;
}

t_header convertirBufferAHeader(int fdCliente, t_tipo_mensaje tipoMensaje, void *msgSerializado, int longitudMensaje) {
	t_header* header = malloc(sizeof(t_header));
	deserializarMensajeABuffer(tipoMensaje, msgSerializado, longitudMensaje, header);
	return *header;
}

int enviarSimple(int fdCliente, void *msg, int len) {

	int bytesleft = len; // cuántos se han quedado pendientes
	int bytes_enviados; //n
	int total = 0;

	while (total < len) {
		bytes_enviados = send(fdCliente, msg + total, bytesleft, 0);
		if (bytes_enviados == -1) {
			if(errno==EINTR){
				continue;
			}
			break;
		}
		total += bytes_enviados;
		bytesleft -= bytes_enviados;
	}
	//len = total; // devuelve aquí la cantidad enviada en realidad

	if (bytes_enviados == -1) {
		perror("[ERROR] Funcion send\bytes_enviados");
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


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void procesarComando(char* comando, fd_set* master, int* fdmax) {

	char* comandoLimpio = string_split(comando, "\n")[0];
	if(string_starts_with(comandoLimpio, "_conectar")) {
		char **elementoPath = string_split(comando, " ");

		while (*elementoPath != NULL) {
			//ultimoElementoEncontrado = string_duplicate(*elementoPath);
			free(*elementoPath);
			elementoPath++;
			char* puerto = string_duplicate(*elementoPath);
			printf("log Conectarse al puerto: %s", puerto);
			free(*elementoPath);
			elementoPath++;
			int socket;
			char* ip = "127.0.0.1";
			if(conectar(ip, puerto, &socket) == 0) {
				FD_SET(socket, master);
				if(socket > *fdmax) {
					*fdmax = socket;
				}
			}

		}
	} else if(string_starts_with(comandoLimpio, "correr programa")) {
		printf("Tendria que reenviar mensaje\n");
	} else {
		puts("Comando no reconocido.\n");
	}

}

void escucharConexiones(char* puerto, int socketServer, int socketMemoria, int socketSwap, int (*funcionParaProcesarMensaje)(int, t_header*, char*, t_tipo_notificacion, void*, t_log*), void* extra,  t_log* logger) {
	fd_set master;    // master file descriptor list
	fd_set read_fds;  // temp file descriptor list for select()
	int fdmax = -1;        // maximum file descriptor number

	int listener;     // listening socket descriptor
	int newfd;        // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	void* buf;    // buffer for client data
	char textbuf[256];
	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int yes = 1;        // for setsockopt() SO_REUSEADDR, below
	int i, rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);

	bool soyServer = !string_equals_ignore_case("0", puerto);

	if (soyServer) {

		// get us a socket and bind it
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; //old tp
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if ((rv = getaddrinfo(NULL, puerto, &hints, &ai)) != 0) {
			fprintf(stderr, "Problemas al escuchar conexiones: %s\n", gai_strerror(rv));
			exit(1);
		}

		for (p = ai; p != NULL; p = p->ai_next) {
			listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (listener < 0) {
				continue;
			}

			// lose the pesky "address already in use" error message
			setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

			if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
				close(listener);
				continue;
			}

			break;
		}

		// if we got here, it means we didn't get bound
		if (p == NULL) {
			fprintf(stderr, "El puerto ya es encuentra utilizado\n");
			exit(2);
		}
		printf("Escuchando nuevas conexiones en el puerto %s\n", puerto);

		freeaddrinfo(ai); // all done with this

		// listen
		if (listen(listener, 10) == -1) {
			perror("listen");
			exit(3);
		}

		// add the listener to the master set
		FD_SET(listener, &master);
	}
	FD_SET(STDIN_FILENO, &master);

	if (socketServer > 0) {
		FD_SET(socketServer, &master);
		if (socketServer > fdmax) {
			fdmax = socketServer;
		}
	}

	if (socketMemoria > 0) {
		FD_SET(socketMemoria, &master);
		if (socketMemoria > fdmax) {
			fdmax = socketMemoria;
		}
	}

	if (socketSwap > 0) {
		FD_SET(socketSwap, &master);
		if (socketSwap > fdmax) {
			fdmax = socketSwap;
		}
	}

	// keep track of the biggest file descriptor
	if (soyServer && listener > fdmax) {

		fdmax = listener; // so far, it's this one
	}

	// main loop
	for (;;) {
		read_fds = master; // copy it
		SELECT:;
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			if (errno==EINTR){
				goto SELECT;
			}
			perror("select");
			exit(4);
		}
		// run through the existing connections looking for data to read
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // we got one!!
				debug("======================================================== inicio ciclo select\n");
				if (soyServer && i == listener) {
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);

					if (newfd == -1) {
						perror("accept");
					} else {
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) {    // keep track of the max
							fdmax = newfd;
						}
						printf("Nueva conexion desde %s en el "
								"socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*) &remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
						funcionParaProcesarMensaje(newfd, NULL, NULL, NEW_CONNECTION, extra, logger);
						/* PRUEBO RECIBIR ALGO SIN TENER QUE INGRESARLO POR CONSOLA
						 int entero;
						 recv(newfd , &entero, sizeof(int),0);
						 printf("el numero recibido es %d \n", entero);
						 */
						//ACA TERMINA LA PRUEBA

					}
				} else {
					// handle data from a client
					if(i == STDIN_FILENO) {
						nbytes = read(i, textbuf, sizeof textbuf);
					} else {
						int tamanioBuffer = sizeof(t_header);
						buf = malloc(tamanioBuffer);
						nbytes = recv(i, buf, tamanioBuffer, MSG_WAITALL);
					}
					if (nbytes <= 0) {
//					if ((nbytes = read(i, buf, sizeof buf)) <= 0) {
						// got error or connection closed by client
						if (nbytes == 0) {
							// connection closed
							printf("De produjo una desconexion desde el socket %d\n", i);
							funcionParaProcesarMensaje(i, NULL, NULL, HANG_UP, extra, logger);
						} else {
							//perror("recv");
							perror("read");
						}
						close(i); // bye!
						FD_CLR(i, &master); // remove from master set
					} else {
						// we got some data from a client
//						printf("En ascii ");
//						int var = 0;
//						for (var = 0; var < string_length(buf); var++) {
//							printf("%d ", buf[var]);
//						}
//						printf("\n");
						if(i == STDIN_FILENO) {
							//printf("Recibi mensaje por socket %d\n", i);
							funcionParaProcesarMensaje(i, NULL, textbuf, TERMINAL_MESSAGE, extra, logger);
						} else {
							t_header header;
							//deserializarMensajeABuffer(HEADER, buf, sizeof(t_header), &header);
							memcpy(&header, buf, sizeof(t_header));
							char* nombre = deserializar_string(i);
							header.nombre = nombre;

							//printf("Recibi mensaje nro. %d por socket %d de %s\n", header.numeroMensaje, i, header.nombre);

							t_resultado_serializacion resultadoSerializacion;
							recibirStructSegunHeader(i, &header, buf, &resultadoSerializacion);
							if(resultadoSerializacion.resultado != NULL) {
								buf = resultadoSerializacion.resultado;
							}
							funcionParaProcesarMensaje(i, &header, buf, MESSAGE, extra, logger);
						}
						/*
						//j desde listener + 1, no quiero la consola, ni el puerto de escucha
						for (j = listener + 1; j <= fdmax; j++) {
							// send to everyone!
							// except the listener and ourselves
							if (j != i) {
								//if (send(j, buf, nbytes, 0) == -1) {
								if (write(j, buf, nbytes) == -1) {
									//perror("send");
									perror("write");
								}
							}
							if (FD_ISSET(j, &master) && !string_starts_with(buf, "log")) {

								if (false && STDIN_FILENO == j && j == socketComando) {
									comando = string_new();
									socketComando = STDIN_FILENO;
									procesandoComando = true;
									string_append(&comando, buf);
									if (strstr(buf, "\n") != NULL) {
										procesarComando(comando, &master, &fdmax);
										procesandoComando = false;
									}
								} else if (false && STDIN_FILENO == j && procesandoComando && j == socketComando) {
									string_append(&comando, buf);
									if (strstr(buf, "\n") != NULL) {
										procesarComando(comando, &master, &fdmax);
										procesandoComando = false;
									}
								} else {
									//Aca manejariamos todos los mensajes
									printf("Recibi mensaje por socket %d \"%s\"\n", j, buf);
								}
							}
						}
						*/
					}
				} // END handle data from client
				debug("======================================================== fin ciclo select\n");
			} // END got new incoming connection
		} // END looping through file descriptors
	} // END for(;;)--and you thought it would never end!

}

void my_log_some(bool info, const char* formato, va_list arguments) {
//	puts("printConsola\n");

	char* nuevo = string_from_vformat(formato, arguments);
	if (info) {
		log_info(logger, nuevo);
	} else {
		log_error(logger, nuevo);
	}
}

void uretardo(unsigned int seconds) {
	retardo(seconds);
}

void retardo(unsigned int seconds) {
	int factor = 1000;
	if(!rapido) {
		factor = factor * 1000;
	}

	factor = factor * (factorRapidez / 100);

	usleep(seconds * factor);
}

void procesarParametros(int argc, char *argv[]) {
	int i;
	for (i = 0; i < argc; ++i) {
		if (string_equals(argv[i], "rapido")) {
			rapido = true;
		} else if (string_ends_with(argv[i], "%")) {
			char** params = string_split(argv[i], "%:");
			char* velocidad = params[0];
			float rapidez = atof(velocidad);
			if(rapidez <= 0) {
				printf("El porcentaje debe ser mayor  a 0.");
			}
			if(rapidez > 100) {
				printf("El porcentaje debe ser menor o igual a 100");
			}
			factorRapidez = rapidez;
		}
	}
}

void my_log_info(const char *formato, ...) {
	va_list arguments;
	va_start(arguments, formato);
	vprintf(formato, arguments);
	va_end(arguments);

	pthread_mutex_lock(&mutexLogs);
	my_log_some(true, formato, arguments);
	pthread_mutex_unlock(&mutexLogs);
}

void my_log_error(const char *formato, ...) {
	va_list arguments;
	va_start(arguments, formato);
	vprintf(formato, arguments);
	va_end(arguments);

	pthread_mutex_lock(&mutexLogs);
	my_log_some(false, formato, arguments);
	pthread_mutex_unlock(&mutexLogs);
}

int conectar(char* ip, char* puerto, int *sock) {
	printf("Conectando a %s:%s\n", ip, puerto);
	struct sockaddr_in dirCent;

	if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	dirCent.sin_family = AF_INET;
	dirCent.sin_port = htons(atoi(puerto));
	dirCent.sin_addr.s_addr = inet_addr(ip);
	memset(&(dirCent.sin_zero), '\0', 8);

	if (connect(*sock, (struct sockaddr *) &dirCent, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		*sock = -1;
		return -1;
	}
	puts("Conexion OK");
	return 0;

}

int defaultProcesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	if(tipoNotificacion == NEW_CONNECTION) {
//		printf("Nueva conexion desde socket %d\n", socket);
	} else {
//		printf("Nuevo mensaje desde socket %d\n", socket);
	}

	if(tipoNotificacion == TERMINAL_MESSAGE) {
		if(buffer != NULL && strstr(buffer, "\n")) {
			if(string_starts_with(buffer, "\n")){
				buffer = "";
			} else {
				while(string_ends_with(buffer, "\n\n")){
					int stringLen = string_length(buffer);
					buffer[stringLen - 1] = '\0';
				}
				char** split = string_split(buffer, "\n");
				buffer = split[0];
			}
		} else {
			debug("Recibi el mensaje por consola: %s\n", buffer);
		}
		if(string_equals(buffer, "debug")) {
			mustDebug = true;
		} else if(string_equals(buffer, "nodebug")) {
			mustDebug = false;
		}
	}
	return 0;
}

t_dictionary* registroSerializadores;

void inicializarRegistroSerializadores() {
	if(registroSerializadores == NULL) {
		registroSerializadores = dictionary_create();

		registrarSerializadores(CONTEXTO_MPROC, "CONTEXTO_MPROC", serializar_CONTEXTO_MPROC, deserializar_CONTEXTO_MPROC);
		registrarSerializadores(HANDSHAKE_ENTRADA_SALIDA, "HANDSHAKE_ENTRADA_SALIDA", serializar_HANDSHAKE_ENTRADA_SALIDA, deserializar_HANDSHAKE_ENTRADA_SALIDA);
		registrarSerializadores(HANDSHAKE_CPU, "HANDSHAKE_CPU", serializar_HANDSHAKE_CPU, deserializar_HANDSHAKE_CPU);
		registrarSerializadores(INICIAR_PROC_SWAP, "INICIAR_PROC_SWAP", serializar_INICIAR_PROC_SWAP, deserializar_INICIAR_PROC_SWAP);
		registrarSerializadores(FIN_PROCESO_SWAP, "FIN_PROCESO_SWAP", serializar_FIN_PROCESO_SWAP, deserializar_FIN_PROCESO_SWAP);
		registrarSerializadores(LEER_SWAP, "LEER_SWAP", serializar_LEER_SWAP, deserializar_LEER_SWAP);
		registrarSerializadores(ERROR_EJECUCION, "ERROR_EJECUCION", serializar_ERROR_EJECUCION, deserializar_ERROR_EJECUCION);
		registrarSerializadores(LEER_MEM, "LEER_MEM", serializar_LEER_MEM, deserializar_LEER_MEM);
		registrarSerializadores(ESCRIBIR_MEM, "ESCRIBIR_MEM", serializar_ESCRIBIR_MEM, deserializar_ESCRIBIR_MEM);
		registrarSerializadores(ESCRIBIR_SWAP, "ESCRIBIR_SWAP", serializar_ESCRIBIR_SWAP, deserializar_ESCRIBIR_SWAP);
		registrarSerializadores(ENTRADA_SALIDA, "ENTRADA_SALIDA", serializar_ENTRADA_SALIDA, deserializar_ENTRADA_SALIDA);
		registrarSerializadores(INICIAR_PROCESO_MEM, "INICIAR_PROCESO_MEM", serializar_INICIAR_PROCESO_MEM, deserializar_INICIAR_PROCESO_MEM);
		registrarSerializadores(FIN_PROCESO_MEM, "FIN_PROCESO_MEM", serializar_FIN_PROCESO_MEM, deserializar_FIN_PROCESO_MEM);
	//+++++++++++++++++++++++ resultados++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		registrarSerializadores(RESUL_INICIAR_PROC_OK, "RESUL_INICIAR_PROC_OK", serializar_RESUL_INICIAR_PROC_OK, deserializar_RESUL_INICIAR_PROC_OK);
		registrarSerializadores(RESUL_INICIAR_PROC_ERROR, "RESUL_INICIAR_PROC_ERROR", serializar_RESUL_INICIAR_PROC_ERROR, deserializar_RESUL_INICIAR_PROC_ERROR);
		registrarSerializadores(RESUL_ESCRIBIR_OK, "RESUL_ESCRIBIR_OK", serializar_RESUL_ESCRIBIR_OK, deserializar_RESUL_ESCRIBIR_OK);
//		registrarSerializadores(RESUL_ESCRIBIR_ERROR, "RESUL_ESCRIBIR_ERROR", serializar_RESUL_ESCRIBIR_ERROR, deserializar_RESUL_ESCRIBIR_ERROR);
		registrarSerializadores(RESUL_INICIAR_PROC_OK_CPU, "RESUL_INICIAR_PROC_OK_CPU", serializar_RESUL_INICIAR_PROC_OK_CPU, deserializar_RESUL_INICIAR_PROC_OK_CPU);
		registrarSerializadores(RESUL_INICIAR_PROC_NO_OK_CPU, "RESUL_INICIAR_PROC_NO_OK_CPU", serializar_RESUL_INICIAR_PROC_NO_OK_CPU, deserializar_RESUL_INICIAR_PROC_NO_OK_CPU);
//		registrarSerializadores(RESUL_LEER_ERROR, "RESUL_LEER_ERROR", serializar_RESUL_LEER_ERROR, deserializar_RESUL_LEER_ERROR);
		registrarSerializadores(RESUL_LEER_OK, "RESUL_LEER_OK", serializar_RESUL_LEER_OK, deserializar_RESUL_LEER_OK);
//		registrarSerializadores(RESUL_FIN_ERROR, "RESUL_FIN_ERROR", serializar_RESUL_FIN_ERROR, deserializar_RESUL_FIN_ERROR);
		registrarSerializadores(RESUL_FIN_OK, "RESUL_FIN_OK", serializar_RESUL_FIN_OK, deserializar_RESUL_FIN_OK);
		registrarSerializadores(RESUL_EJECUCION_OK, "RESUL_EJECUCION_OK", serializar_RESUL_EJECUCION_OK, deserializar_RESUL_EJECUCION_OK);
		registrarSerializadores(RESUL_EJECUCION_ERROR, "RESUL_EJECUCION_ERROR", serializar_RESUL_EJECUCION_ERROR, deserializar_RESUL_EJECUCION_ERROR);
		registrarSerializadores(RESUL_LEER_OK, "RESUL_LEER_OK", serializar_RESUL_LEER_OK, deserializar_RESUL_LEER_OK);
		registrarSerializadores(RESUL_FIN, "RESUL_FIN", serializar_RESUL_FIN, deserializar_RESUL_FIN);
		registrarSerializadores(RESUL_LEER_OK_CPU, "RESUL_LEER_OK_CPU", serializar_RESUL_LEER_OK_CPU, deserializar_RESUL_LEER_OK_CPU);
		registrarSerializadores(SOBREESCRIBIR_SWAP, "SOBREESCRIBIR_SWAP", serializar_SOBREESCRIBIR_SWAP, deserializar_SOBREESCRIBIR_SWAP);
		registrarSerializadores(RESUL_SOBREESCRIBIR_OK, "RESUL_SOBREESCRIBIR_OK", serializar_RESUL_SOBREESCRIBIR_OK, deserializar_RESUL_SOBREESCRIBIR_OK);
		registrarSerializadores(LEER_SWAP_POR_ESCRIBIR, "LEER_SWAP_POR_ESCRIBIR", serializar_LEER_SWAP_POR_ESCRIBIR, deserializar_LEER_SWAP_POR_ESCRIBIR);
		registrarSerializadores(RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR, "RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR", serializar_RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR, deserializar_RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR);
		registrarSerializadores(RESUL_TRAER_PAG_SWAP_OK, "RESUL_TRAER_PAG_SWAP_OK", serializar_RESUL_TRAER_PAG_SWAP_OK, deserializar_RESUL_TRAER_PAG_SWAP_OK);
		registrarSerializadores(RESUL_ESCRIBIR, "RESUL_ESCRIBIR", serializar_RESUL_ESCRIBIR, deserializar_RESUL_ESCRIBIR);
  registrarSerializadores(TIEMPO_CPU_RESUL,"TIEMPO_CPU_RESUL", serializar_TIEMPO_CPU_RESUL, deserializar_TIEMPO_CPU_RESUL);
  registrarSerializadores(TIEMPO_CPU,"TIEMPO_CPU", serializar_TIEMPO_CPU, deserializar_TIEMPO_CPU);
		registrarSerializadores(NOTIFICACION_HILO_ENTRADA_SALIDA,"NOTIFICACION_HILO_ENTRADA_SALIDA", serializar_NOTIFICACION_HILO_ENTRADA_SALIDA, deserializar_NOTIFICACION_HILO_ENTRADA_SALIDA);
	}
}


bool existeSerializacion(t_tipo_mensaje tipoMensaje) {
	inicializarRegistroSerializadores();
	char* keySerializacion = generarKeySerializacion(tipoMensaje);
	return dictionary_has_key(registroSerializadores, keySerializacion);
}

char* generarKeySerializacion(t_tipo_mensaje tipoMensaje) {
	return string_from_format("serializacion(%d)", tipoMensaje);
}

t_registro_serializacion* getSerializacion(t_tipo_mensaje tipoMensaje) {
	inicializarRegistroSerializadores();
	char* keySerializacion = generarKeySerializacion(tipoMensaje);
	return (t_registro_serializacion*) dictionary_get(registroSerializadores, keySerializacion);
}

void registrarSerializadores(t_tipo_mensaje tipoMensaje, char* descripcion, void* funcionSerializacion, void* funcionDeserializacion) {
	char* keySerializacion = generarKeySerializacion(tipoMensaje);
	t_registro_serializacion* registroSerializacion = malloc(sizeof(t_registro_serializacion));
	registroSerializacion->tipoMensaje = tipoMensaje;
	registroSerializacion->descripcion = descripcion;
	registroSerializacion->funcionSerializacion = funcionSerializacion;
	registroSerializacion->funcionDeserializacion = funcionDeserializacion;
	dictionary_put(registroSerializadores, keySerializacion, registroSerializacion);
}

char* getNombreTipoMensaje(t_tipo_mensaje tipoMensaje) {
	char* keySerializacion = generarKeySerializacion(tipoMensaje);
	t_registro_serializacion* registroSerializacion = (t_registro_serializacion *)dictionary_get(registroSerializadores, keySerializacion);
	if(registroSerializacion == NULL) {
		return "TIPO_MENSAJE_SIN_NOMBRE";
	}
	return registroSerializacion->descripcion;
}

int enviarSerializado(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	t_registro_serializacion* serializacion = getSerializacion(tipoMensaje);

	//creamos el header
	t_header header = crearHeader(tipoMensaje, NULL, 0);
	enviarHeader(fdCliente, header);
	serializar_string(fdCliente, header.nombre);

	void* funcion = serializacion->funcionSerializacion;

	return ejecutarSerializacion(funcion, fdCliente, header, estructura);
}

int recibirSerializado(int fdCliente, t_header header, void* estructura, t_resultado_serializacion* resultadoSerializacion) {
	t_registro_serializacion* serializacion = getSerializacion(header.tipoMensaje);

	void* funcion = serializacion->funcionDeserializacion;
	return ejecutarDeserializacion(funcion, fdCliente, header, resultadoSerializacion);
}

int ejecutarSerializacion(void* (*funcion)(int, t_tipo_mensaje, void*), int fdCliente, t_header header, void* estructura) {
	debug("Envio  %-35s       desde %-12s   Id_msg: %5d\n", getNombreTipoMensaje(header.tipoMensaje), header.nombre, header.numeroMensaje);
	(int*)funcion(fdCliente, header.tipoMensaje, estructura);
	//TODO
	return 0;
}

int ejecutarDeserializacion(void* (*funcion)(int, t_tipo_mensaje), int fdCliente, t_header header, t_resultado_serializacion* resultadoDeserializacion) {
	debug("Recibo %-35s       desde %-12s   Id_msg: %5d\n", getNombreTipoMensaje(header.tipoMensaje), header.nombre, header.numeroMensaje);
	void* resultado = funcion(fdCliente, header.tipoMensaje);
	resultadoDeserializacion -> resultado = resultado;
	//TODO
	return 0;
}

bool mustDebug = false;
bool rapido = false;
float factorRapidez = 100.0;

void debug(const char *formato, ...) {
//	puts("printConsola\n");
	va_list arguments;
	va_start(arguments, formato);
//		int res = vprintf(formato, arguments);
	va_end(arguments);

	char* nuevo = string_from_vformat(formato, arguments);
	log_debug(logger, nuevo);
	if (mustDebug) {
		puts(nuevo);
	}
}

