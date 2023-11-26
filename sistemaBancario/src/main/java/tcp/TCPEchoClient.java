package tcp;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Scanner;

public class TCPEchoClient {
    private static final int BUFSIZE = 256;
    public static final String ANSI_RESET = "\u001B[0m";

    // Declaring the color
    // Custom declaration
    public static final String ANSI_GREEN = "\u001B[32m";

    public static final String ANSI_RED = "\u001B[31m";

    public static void main(String[] args) throws IOException {
        if ((args.length < 2) || (args.length > 3))
            throw new IllegalArgumentException("Parameter(s): <Server> <Word> [<Port>]");
        String server = args[0];

        // Convert input String to bytes using default character encoding
        byte[] byteBuffer = args[1].getBytes();
        int servPort = (args.length == 3) ? Integer.parseInt(args[2]) : 7;
        System.out.println("Before connecting " + server + " " + servPort);

        // Create socket that is connected to server on specified port
        try (Socket socket = new Socket(server, servPort)) {
            System.out.println("Connected to server... sending echo string");
            InputStream in = socket.getInputStream();
            OutputStream out = socket.getOutputStream();

            // Send the encoded string to the server
            out.write(byteBuffer);

            // Receive and print the response from the server in a loop
            byte[] receiveBuffer = new byte[BUFSIZE];
            int bytesRead;
            Scanner scanner = new Scanner(System.in);
            int inicio = 0;
            byte[] userMessage;

            while ((bytesRead = in.read(receiveBuffer)) != -1) {
                String receivedMessage = new String(receiveBuffer, 0, bytesRead);
                if (receivedMessage.contains("fim")) {
                    System.out.println(ANSI_GREEN + "O Banco BCC agradece a sua preferencia, foi um prazer te atender! \n" + ANSI_RESET);
                    break;
                }
                System.out.println("Received:\n " + receivedMessage);
                if (inicio == 0){
                    inicio = 1;
                    System.out.println("Digite o ID da conta que você deseja acessar: ");
                    // Lê a entrada do usuário e envia para o servidor
                    System.out.print("Digite a mensagem para enviar ao servidor: ");
                    String userInput = scanner.nextLine();

                    // Envia a mensagem do usuário para o servidor
                    userMessage = userInput.getBytes();
                    out.write(userMessage);
                    out.flush();  // Certifique-se de que a mensagem seja enviada imediatamente
                }else{
                    System.out.println("Escolha a operação que deseja realizar:\n 1: Visualizar Saldo\n 2: Realizar Saque\n 3: Realizar Depósito\n 4: Realizar Transferência\n 0: SAIR\n");
                    // Lê a entrada do usuário e envia para o servidor
                    System.out.print("Digite a mensagem para enviar ao servidor: ");
                    String userInput = scanner.nextLine();
                    if(userInput.equals("2")){
                        System.out.print("Digite o valor do saque: ");
                        String valorSaque = scanner.nextLine();
                        userInput = userInput + "/" + valorSaque;
                        userMessage = userInput.getBytes();
                        out.write(userMessage);
                        out.flush();  // Certifique-se de que a mensagem seja enviada imediatamente
                    } else if(userInput.equals("3")){
                        System.out.print("Digite o valor do depósito: ");
                        String valorDeposito = scanner.nextLine();
                        userInput = userInput + "/" + valorDeposito;
                        userMessage = userInput.getBytes();
                        out.write(userMessage);
                        out.flush();  // Certifique-se de que a mensagem seja enviada imediatamente
                    }else if(userInput.equals("4")){
                        System.out.print("Digite o valor de transferência: ");
                        String valorTrans = scanner.nextLine();
                        System.out.print("Digite o ID da conta que deseja transferir: ");
                        String idTrans = scanner.nextLine();
                        userInput = userInput + "/" + valorTrans + "/" + idTrans;
                        userMessage = userInput.getBytes();
                        out.write(userMessage);
                        out.flush();  // Certifique-se de que a mensagem seja enviada imediatamente
                    }else {


                        // Envia a mensagem do usuário para o servidor
                        userMessage = userInput.getBytes();
                        out.write(userMessage);
                        out.flush();  // Certifique-se de que a mensagem seja enviada imediatamente
                    }
                }


            }
        }
    }
}
