package sistemaBancario;


import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Scanner;

public class Client {
    // Declaring ANSI_RESET so that we can reset the color
    public static final String ANSI_RESET = "\u001B[0m";

    // Declaring the color
    // Custom declaration
    public static final String ANSI_GREEN = "\u001B[32m";

    public static final String ANSI_RED = "\u001B[31m";
    public static void  main(String[] args){
        try {
            Banco stub = (Banco) Naming.lookup("rmi://localhost:1099/Banco");
            //System.out.println(stub.digaOla());
            System.out.println("Digite o ID da conta que você deseja acessar: ");
            Scanner scanner = new Scanner(System.in);
            String userInput = scanner.nextLine();
            int idUser = Integer.parseInt(userInput);
            String name = stub.getName(idUser);
            System.out.println(ANSI_GREEN + "O Cliente com ID " + idUser + " é o: " + name + ANSI_RESET);
            boolean online = true;
            while(online){
                System.out.println("Escolha a operação que deseja realizar:\n 1: Visualizar Saldo\n 2: Realizar Saque\n 3: Realizar Depósito\n 4: Realizar Transferência\n 0: SAIR\n");
                switch (Integer.parseInt(scanner.nextLine())) {
                    case 0 -> {
                        System.out.println(ANSI_RED + " SAIR! "+ ANSI_RESET);
                        System.out.println(ANSI_GREEN + "O Banco BCC agradece a sua preferência, foi um prazer te atender! \n" + ANSI_RESET);
                        online = false;
                    }
                    case 1 -> {
                        System.out.println(ANSI_RED +" SALDO "+ ANSI_RESET);
                        double saldo = stub.saldo(idUser);
                        System.out.println(ANSI_GREEN + "Saldo do cliente com ID " + idUser + ": R$" + saldo+"\n" + ANSI_RESET);

                    }
                    case 2 -> {
                        System.out.println(ANSI_RED +" SAQUE "+ ANSI_RESET);
                        System.out.print("Digite o valor do saque: ");
                        double valorSaque = Double.parseDouble(scanner.nextLine());
                        stub.sacar(valorSaque, idUser);
                        System.out.println(ANSI_GREEN + "Foi realizado o saque na conta do cliente com ID " +idUser+ " no valor de: R$" + valorSaque+"\n"+ ANSI_RESET);

                    }
                    case 3 -> {
                        System.out.println(ANSI_RED+" DEPÓSITO"+ ANSI_RESET);
                        System.out.print("Digite o valor do depósito: ");
                        double valorDeposito = Double.parseDouble(scanner.nextLine());
                        stub.depositar(valorDeposito, idUser);
                        System.out.println(ANSI_GREEN +"Foi realizado o deposito na conta do cliente com ID " +idUser+ " no valor de: R$" + valorDeposito+"\n"+ ANSI_RESET);
                    }
                    case 4 -> {
                        System.out.println(ANSI_RED+" TRANSFERIR PARA OUTRA CONTA"+ ANSI_RESET);
                        System.out.print("Digite o valor de transferência: ");
                        double valorTransferencia = Double.parseDouble(scanner.nextLine());
                        System.out.print("Digite o id da conta que deseja transferir: ");
                        int idConta = Integer.parseInt(scanner.nextLine());
                        stub.sacar(valorTransferencia, idUser);
                        stub.depositar(valorTransferencia, idConta);

                        System.out.println(ANSI_GREEN +"Foi realizado a transferência da conta de ID " +idUser +" para a conta de ID "+idConta + " no valor de: R$" + valorTransferencia +"\n"+ ANSI_RESET);
                    }
                    default -> {
                        System.out.println("Opção não reconhecida.");
                        System.out.println(ANSI_RED + "Opção não reconhecida, tente novamente."+"\n"+ ANSI_RESET);
                    }
                }
            }

        }catch (RemoteException | MalformedURLException | NotBoundException e){
            e.printStackTrace();
        }
    }
}
