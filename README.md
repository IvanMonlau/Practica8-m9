# Practica8-m9

Servidor
El servidor és l'encarregat de gestionar les comandes dels clients. Les seves principals funcions són:
1.	Creació del socket: El servidor crea un socket d'escolta que permet acceptar connexions entrants dels clients.
2.	Acceptació de connexions: El servidor accepta les connexions dels clients i crea un nou socket per a cada client connectat.
3.	Gestió de comandes: El servidor rep les comandes dels clients, les assigna un identificador únic i les guarda en un fitxer de text.
4.	Enviament de confirmació: El servidor envia als clients la confirmació de recepció de la comanda, juntament amb el seu identificador únic.
5.	Gestió de múltiples clients: El servidor utilitza fils per gestionar múltiples clients de manera simultània. Això permet que el servidor pugui atendre les comandes de diversos clients sense bloquejar-se.

   
Client
El client és l'encarregat d'enviar comandes al servidor. Les seves principals funcions són:
1.	Creació del socket: El client crea un socket per connectar-se al servidor.
2.	Connexió al servidor: El client es connecta al servidor a través del port especificat.
3.	Enviament de comandes: El client envia la comanda de l'usuari al servidor.
4.	Recepció de confirmació: El client rep la confirmació del servidor, juntament amb l'identificador únic de la comanda.
5.	Mostra de la confirmació: El client mostra a l'usuari la confirmació rebuda.
