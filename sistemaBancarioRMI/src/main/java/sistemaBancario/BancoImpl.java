package sistemaBancario;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.List;

public class BancoImpl extends UnicastRemoteObject implements Banco {

    List<Usuario> listaUsuario = new ArrayList<>();


    protected BancoImpl() throws RemoteException {
        super();
        listaUsuario.add(new Usuario(1, "João", "Silva", 1000.0));
        listaUsuario.add(new Usuario(2, "Maria", "Santos", 1500.0));
        listaUsuario.add(new Usuario(3, "Pedro", "Oliveira", 800.0));
    }
    @Override
    public void depositar(double valor, int idDesejado) throws RemoteException {
        for (Usuario usuario: listaUsuario) {
            if (usuario.getId() == idDesejado) {
                if (valor > 0) {
                    usuario.saldo += valor;
                    System.out.println("Depósito de R$" + valor + " realizado com sucesso.");
                } else {
                    System.out.println("Valor de depósito inválido.");
                }
            }
        }

    }

    @Override
    public void sacar(double valor, int idDesejado) throws RemoteException {
        for (Usuario usuario: listaUsuario) {
            if (usuario.getId() == idDesejado) {
                if (valor > 0 && valor <= usuario.saldo) {
                    usuario.saldo -= valor;
                    System.out.println("Saque de R$" + valor + " realizado com sucesso.");
                } else {
                    System.out.println("Valor de saque inválido ou saldo insuficiente.");
                }
            }
        }

    }

    @Override
    public double saldo(int idDesejado) throws RemoteException {
        double saldoUser = 0;
        for (Usuario usuario: listaUsuario) {
            if (usuario.getId() == idDesejado) {
                saldoUser =  usuario.saldo;
            }
        }
        return saldoUser;
    }

    public String getName(int idDesejado)throws RemoteException{
        String name = "";
        for (Usuario usuario: listaUsuario) {
            if (usuario.getId() == idDesejado) {
                name = usuario.getNome() + " " + usuario.getSobrenome();
            }
        }
        return name;
    }



}
