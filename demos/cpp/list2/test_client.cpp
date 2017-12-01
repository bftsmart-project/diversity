#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-client.hpp>
#include <Request.pb.h>
#include <Response.pb.h>
#include <Estado.pb.h>

int
main (int argc, char *argv[])
{

    if (argc == 1) {
	printf ("Usage: %s id_replica classpath_java\n", argv[0]);
	return -1;
    }

    if (argc < 3) {
	printf ("%s", "Argumentos invalidos.\n");
	return -1;
    }

    bftsmart::BftSmartClient bc (atoi (argv[1]), argv[2]);

    BFT_BYTE saida[10000];

    int cmd, result;

    do {
	printf ("%s\n", "1-add");
	printf ("%s\n", "2-get");
	printf ("%s\n", "3-remove");
	printf ("%s\n", "4-contains");
	printf ("%s\n", "5-size");
	printf ("%s\n", "6-FINALIZAR. pressione ctrl-c depois.");
	scanf ("%d", &cmd);
	getchar ();
	bftbench::Request req;
	unsigned int tamanho;
	BFT_BYTE *out;

	bftbench::Response rsp;

	switch (cmd) {

	case 5:
	    req.set_action (bftbench::Request::SIZE);
	    tamanho = req.ByteSize ();
	    out = (BFT_BYTE *) malloc (tamanho);

	    req.SerializeToArray ((void *) out, tamanho);

	    result = invokeOrdered (out, tamanho, saida);

	    rsp.ParseFromArray ((const void *) saida, result);
	    printf ("%s%d\n", "response:", rsp.intresponse ());
	    break;

	case 2:
	    printf ("%s\n", "Digite o valor:");
	    scanf ("%d", &cmd);
	    getchar ();

	    req.set_action (bftbench::Request::GET);
	    req.set_value (cmd);
	    tamanho = req.ByteSize ();

	    out = (BFT_BYTE *) malloc (tamanho);

	    req.SerializeToArray ((void *) out, tamanho);

	    result = invokeOrdered (out, tamanho, saida);
	    rsp.ParseFromArray ((const void *) saida, result);
	    printf ("%s%d\n", "response:", rsp.intresponse ());
	    break;
	case 4:
	    printf ("%s\n", "Digite o valor:");
	    scanf ("%d", &cmd);
	    getchar ();
	    req.set_action (bftbench::Request::CONTAINS);
	    req.set_value (cmd);
	    tamanho = req.ByteSize ();

	    out = (BFT_BYTE *) malloc (tamanho);

	    result = invokeOrdered (out, tamanho, saida);
	    rsp.ParseFromArray ((const void *) saida, result);
	    printf ("%s%d\n", "response:", rsp.boolresponse ());
	    break;

	case 3:
	    printf ("%s\n", "Digite o valor:");
	    scanf ("%d", &cmd);
	    getchar ();
	    req.set_action (bftbench::Request::REMOVE);
	    req.set_value (cmd);
	    tamanho = req.ByteSize ();
	    out = (BFT_BYTE *) malloc (tamanho);
	    result = invokeOrdered (out, tamanho, saida);
	    rsp.ParseFromArray ((const void *) saida, result);
	    printf ("%s%d\n", "response:", rsp.boolresponse ());
	    break;

	case 1:
	    printf ("%s\n", "Digite o valor:");
	    scanf ("%d", &cmd);
	    getchar ();
	    req.set_action (bftbench::Request::ADD);
	    req.set_value (cmd);
	    tamanho = req.ByteSize ();
	    out = (BFT_BYTE *) malloc (tamanho);
	    result = invokeOrdered (out, tamanho, saida);
	    rsp.ParseFromArray ((const void *) saida, result);
	    printf ("%s%d\n", "response:", rsp.boolresponse ());
	    break;
	default:
	    break;
	}
    } while (cmd != 6);
    return 0;
}
