package sistemaBancario;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Banco extends Remote {
    void depositar(double valor, int idDesejado) throws RemoteException;
    void sacar(double valor, int idDesejado) throws RemoteException;
    double saldo(int idDesejado) throws RemoteException;

    String getName(int idDesejado) throws RemoteException;

}
