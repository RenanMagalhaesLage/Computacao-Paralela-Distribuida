package sistemaBancario;


import sistemaBancario.BancoImpl;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Server {

    public static void  main(String[] args) throws RemoteException {
        try {
            Registry registry = LocateRegistry.createRegistry(1099);
            Naming.rebind("rmi://localhost:1099/Banco", new BancoImpl());
            System.out.println("Servidor levantou...");
        }catch (RemoteException | MalformedURLException e){
            e.printStackTrace();
        }
    }
}
