package bftsmart.diversity.demos.kv;
import bftbench.RequestOuterClass.Request;
import bftbench.ResponseOuterClass.Response;
import bftsmart.tom.ServiceProxy;
import java.io.*;
import com.google.protobuf.InvalidProtocolBufferException;


public class KVClientProto {
public static void main(String[] args) throws IOException {

	if (args.length < 1) {
		System.out.println("Usage: ... ListClient <process id>");
		System.exit(-1);
}

int id = Integer.parseInt(args[0]);

ServiceProxy proxy;
proxy = new ServiceProxy(id);
InputStreamReader converter = new InputStreamReader(System.in);
BufferedReader in = new BufferedReader(converter);
String lido, lido2;
do {
System.out.println("digite operacao ( get, put, delete, list, s(sair) ):");
lido = in.readLine();

String chave = "";
String valor = "";
if (lido.equals("put")) {
System.out.println("digite o chave:");
lido2 = in.readLine();
chave = lido2;

System.out.println("digite o valor:");
lido2 = in.readLine();
valor = lido2;
} else if (lido.equals("get") || lido.equals("delete")) {
System.out.println("digite o chave:");
lido2 = in.readLine();
chave = lido2;

}
Request.Builder reqst;
byte[] msg,rsp;
Response resp;
switch (lido)
{
case "put":

reqst = Request.newBuilder();

reqst.setAction(Request.RequestType.PUT);
reqst.setKey(chave);
reqst.setValue(valor);

 msg = reqst.build().toByteArray();
System.out.println("--");
//System.out.println(msg.length);


rsp = proxy.invokeOrdered(msg);
 resp = Response.parseFrom(rsp);
System.out.println(resp.getBoolResponse());

//byte[] rsp2 = proxy.invokeUnordered(msg);
//bftbench.VazioOuterClass.Vazio.parseFrom(rsp2);
//System.out.println(rsp2.length);
  break;
case "get":

reqst = Request.newBuilder();

reqst.setAction(Request.RequestType.GET);
reqst.setKey(chave);

msg = reqst.build().toByteArray();
System.out.println("--");
//System.out.println(msg.length);


rsp = proxy.invokeOrdered(msg);
 resp = Response.parseFrom(rsp);
System.out.println(resp.getBoolResponse());
if (resp.getBoolResponse()) {
System.out.println(resp.getStringResponse());
}

  break;
case "delete":

reqst = Request.newBuilder();

reqst.setAction(Request.RequestType.DELETE);
reqst.setKey(chave);

 msg = reqst.build().toByteArray();
System.out.println("--");
//System.out.println(msg.length);


 rsp = proxy.invokeOrdered(msg);
 resp = Response.parseFrom(rsp);
System.out.println(resp.getBoolResponse());

  break;
case "list":

reqst = Request.newBuilder();

reqst.setAction(Request.RequestType.LIST);


msg = reqst.build().toByteArray();
System.out.println("--");
//System.out.println(msg.length);


 rsp = proxy.invokeOrdered(msg);
 resp = Response.parseFrom(rsp);
for (String str : resp.getListResponseList())
{
System.out.println(str);
}
  break;

default: 
  break;
}

} while (lido != "s");

proxy.close();

}

}
