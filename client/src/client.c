#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	int puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger, "Hola! soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config,"IP");
	puerto = config_get_int_value(config,"PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "IP:%s", ip);
	log_info(logger, "PUERTO:%d",puerto);
	log_info(logger, "VALOR:%s", valor);
	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	enviar_mensaje("Hola! soy el cliente", conexion );

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create(LOG_FILE, LOG_NAME, false, LOG_LEVEL_INFO );;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create(CONFIG_FILE)) == NULL){
		printf("No pude leer la config\n");
		exit(2);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{

	char* leido;
	int leiCaracterSalida;
	do{
		leido = readline("cli> ");
		leiCaracterSalida = strcmp(leido, CARACTER_SALIDA);
		if(leiCaracterSalida!=0) log_info(logger,"cli:> %s",leido);
		free(leido);
	}while(leiCaracterSalida);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	int leiCaracterSalida;
	do{
		leido = readline("cli> ");
		leiCaracterSalida = strcmp(leido, CARACTER_SALIDA);
		if(leiCaracterSalida!=0) agregar_a_paquete(paquete,leido,strlen(leido)+1);
		free(leido);
	}while(leiCaracterSalida);
	enviar_paquete(paquete,conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_info(logger, "Terminando programa cliente");
	log_destroy(logger);
	if(config!=NULL) config_destroy(config);
	liberar_conexion(conexion);
}
