#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_info(logger, "El cliente se desconecto.");
			int opcion;
			printf("¿Desea mantener el servidor corriendo? 1-Si 0-No\n");
			scanf("%d", &opcion);
			if(opcion==1){
				if (recibir_operacion(cliente_fd)==SIN_CLIENTES) {
					log_info(logger, "No hay mas clientes conectados, se cierra el servidor.");
					close(server_fd);
					return EXIT_SUCCESS;
				}
				log_info(logger,"Servidor continua corriendo...");
				cliente_fd = esperar_cliente(server_fd);

			}else if(opcion==0){
				log_info(logger,"Terminando servidor...");
				close(server_fd);
				return EXIT_SUCCESS;
			}else{
				log_error(logger,"Opcion invalida. Terminando servidor...");
				return EXIT_FAILURE;
			}
			break;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
