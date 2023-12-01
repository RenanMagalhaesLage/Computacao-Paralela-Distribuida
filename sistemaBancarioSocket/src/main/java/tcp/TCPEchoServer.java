package tcp;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.net.*; // for Socket, ServerSocket
import java.io.*; // for IOException and Input/OutputStream



public class TCPEchoServer {
    // Declaring ANSI_RESET so that we can reset the color
    public static final String ANSI_RESET = "\u001B[0m";

    // Declaring the color
    // Custom declaration
    public static final String ANSI_GREEN = "\u001B[32m";

    public static final String ANSI_RED = "\u001B[31m";

    private static final int BUFSIZE = 128; // Size of receive buffer
    public static void main (String[] args) throws IOException {

        List<Cliente> listaClientes = new ArrayList<>();

        listaClientes.add(new Cliente(1, "João", "Silva", 1000.0));
        listaClientes.add(new Cliente(2, "Maria", "Santos", 1500.0));
        listaClientes.add(new Cliente(3, "Pedro", "Oliveira", 800.0));

        if (args.length != 1)
            throw new IllegalArgumentException("Parameter(s): <Port>");
        try {
            InetAddress address = InetAddress.getLocalHost();
            System.out.println("Local Host:");
            System.out.println("\t" + address.getHostName());
            System.out.println("\t" + address.getHostAddress());
        } catch (UnknownHostException e) {
            System.out.println("Unable to determine this host's address");
        }

        int servPort = Integer.parseInt(args[0]);
        // Create a server socket to accept client connection requests
        ServerSocket servSock = new ServerSocket(servPort);
        int recvMsgSize; // Size of receive message
        byte[] byteBuffer = new byte[BUFSIZE]; // Receive buffer
        for (;;) { // Run forever, accpeting and servicing connections
            Socket clntSock = servSock.accept(); // Get client connection
            System.out.println("Handling client at " +
                    clntSock.getInetAddress().getHostAddress() + " on port " +
                    clntSock.getPort());
            try (InputStream in = clntSock.getInputStream();
                 OutputStream out = clntSock.getOutputStream()) {
                Scanner scanner = new Scanner(System.in);


                int idDesejado= 1;
                int number_operacao;


                String message = "Welcome to the server!\n";
                out.write(message.getBytes());
                out.flush();
                String clientMessage;

                int inicio = 0;
                int fim = 0;

                // Receive until client closes connection,
                // indicated by -1 return
                while ((recvMsgSize = in.read(byteBuffer)) != -1){
                    if (fim == 1) {
                        break;
                    }
                    //message = "Escolha a operação que deseja realizar:\n 1: Visualizar Saldo\n 2: Realizar Saque\n 3: Realizar Depósito\n 4: Realizar Transferência\n 0: SAIR\n";
                    //out.write(message.getBytes());
                    //out.flush();
                    clientMessage = new String(byteBuffer, 0, recvMsgSize);
                    System.out.println(clientMessage);

                    String[] partes = clientMessage.split("/");
                    if (partes[0].matches("\\d+")) {
                        number_operacao = Integer.parseInt(partes[0]);

                        if(inicio == 0){
                            inicio = 1;
                            idDesejado = number_operacao;
                            for (Cliente cliente : listaClientes) {
                                if (cliente.getId() == idDesejado) {
                                    message = ANSI_GREEN + "O ID escolhido pertence à "+ cliente.getNome() +" " + cliente.getSobrenome() + "\n"+ ANSI_RESET;
                                    out.write(message.getBytes());
                                    out.flush();
                                    System.out.println("O ID escolhido pertence à "+ cliente.getNome() +" " + cliente.getSobrenome() + "\n");
                                    break;
                                }
                            }
                        }else{
                            switch (number_operacao) {
                                case 0 -> {
                                    System.out.println(ANSI_RED + " SAIR! "+ ANSI_RESET);
                                    message =
                                    message = "fim";
                                    out.write(message.getBytes());
                                    out.flush();
                                    scanner.close();
                                    fim = 1;
                                }
                                case 1 -> {
                                    System.out.println(ANSI_RED +" SALDO "+ ANSI_RESET);
                                    for (Cliente cliente : listaClientes) {
                                        if (cliente.getId() == idDesejado) {
                                            double saldoCliente = cliente.getSaldo();
                                            message = ANSI_GREEN + "Saldo do cliente com ID " + idDesejado + ": R$" + saldoCliente+"\n" + ANSI_RESET;
                                            out.write(message.getBytes());
                                            out.flush();
                                            break; // Se encontrar o cliente, saia do loop
                                        }
                                    }
                                }
                                case 2 -> {
                                    System.out.println(ANSI_RED +" SAQUE "+ ANSI_RESET);
                                    //System.out.print("Digite o valor do saque: ");
                                    //double valorSaque = scanner.nextDouble();
                                    double valorSaque = Double.parseDouble(partes[1]);
                                    for (Cliente cliente : listaClientes) {
                                        if (cliente.getId() == idDesejado) {
                                            cliente.sacar(valorSaque);
                                            message = ANSI_GREEN + "Foi realizado o saque na conta do cliente com ID " +idDesejado+ " no valor de: R$" + valorSaque+"\n"+ ANSI_RESET;
                                            out.write(message.getBytes());
                                            out.flush();
                                            break; // Se encontrar o cliente, saia do loop
                                        }
                                    }
                                }
                                case 3 -> {
                                    System.out.println(ANSI_RED+" DEPÓSITO"+ ANSI_RESET);
                                    //System.out.print("Digite o valor do depósito: ");
                                    double valorDeposito = Double.parseDouble(partes[1]);
                                    for (Cliente cliente : listaClientes) {
                                        if (cliente.getId() == idDesejado) {
                                            cliente.depositar(valorDeposito);
                                            message = ANSI_GREEN +"Foi realizado o deposito na conta do cliente com ID " +idDesejado+ " no valor de: R$" + valorDeposito+"\n"+ ANSI_RESET;
                                            out.write(message.getBytes());
                                            out.flush();
                                            break; // Se encontrar o cliente, saia do loop
                                        }
                                    }
                                }
                                case 4 -> {
                                    System.out.println(ANSI_RED+" TRANSFERIR PARA OUTRA CONTA"+ ANSI_RESET);
                                    //System.out.print("Digite o valor de transferência: ");
                                    double valorTransferencia = Double.parseDouble(partes[1]);
                                    //System.out.print("Digite o id da conta que deseja transferir: ");
                                    int idConta = Integer.parseInt(partes[2]);
                                    for (Cliente cliente : listaClientes) {
                                        if (cliente.getId() == idDesejado) {
                                            cliente.sacar(valorTransferencia);
                                        }
                                        if (cliente.getId() == idConta) {
                                            cliente.depositar(valorTransferencia);
                                        }
                                    }
                                    message = ANSI_GREEN +"Foi realizado a transferência da conta de ID " +idDesejado+ " para a conta de ID "+idConta + " no valor de: R$" + valorTransferencia +"\n"+ ANSI_RESET;
                                    out.write(message.getBytes());
                                    out.flush();
                                }
                                default -> {
                                    System.out.println("Opção não reconhecida.");
                                    message = ANSI_RED + "Opção não reconhecida, tente novamente."+"\n"+ ANSI_RESET;
                                    out.write(message.getBytes());
                                    out.flush();
                                }
                            }
                        }


                    }
                    //out.write(byteBuffer, 0, recvMsgSize);
                }
            }
            clntSock.close(); // Close de socket.
            // We are done with this client
        }
        /* NOT REACHED */
    }
}