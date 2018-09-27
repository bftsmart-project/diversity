/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bftsmart.diversity.demos.list;

/**
 *
 * @author CaioYuri
 */
public class ListaNormal {
    public ElementoLista inicio;
    public ElementoLista fim;
    
    public ListaNormal() {
        this.inicio = null;
        this.fim = null;
       
    }
    
    public int insereInicio(int valor)
    {
        ElementoLista novo = new ElementoLista();
        novo.dado = valor;
        novo.proximo = this.inicio;
        this.inicio = novo;
        if (this.fim == null)
            this.fim = this.inicio;
        return 0;
    }
    
    public int insereFinal(int valor)
    {
        ElementoLista novo = new ElementoLista();
        novo.dado = valor;
        novo.proximo = null;
        if (this.inicio == null)
            this.inicio = novo;
        else
            this.fim.proximo = novo;
        this.fim = novo;
        return 0;                   
        
    }
    
    public int estaVazia()
    {
        if (this.inicio == null)
            return 1;
        return 0;
    }
    
    public int removeInicio()
    {
        if (this.inicio == null)
            return -1;
        int tmp = this.inicio.dado;
        ElementoLista removido = this.inicio;
        this.inicio = this.inicio.proximo;
        if ( this.inicio == null )
            this.fim = null;
        return tmp;
                   
    }
    
    public int removeFinal()
    {
        if (this.inicio == null)
            return -1;
        if (this.inicio == this.fim) {
            int tmp = this.inicio.dado;
            this.inicio = null;
            this.fim = null;
            return tmp;
            
        }
        ElementoLista penultimo = this.inicio;
        while (penultimo.proximo != this.fim)
        {
            penultimo = penultimo.proximo;
        }
        int tmp = penultimo.proximo.dado;
        this.fim = penultimo;
        penultimo.proximo = null;
        return tmp;
                 
    }
    
    public int remover(int pos)
    {
        if (this.inicio == null)
        {
            return -1;
        }
        if (pos < 0)
            return -1;
        ElementoLista removido = this.inicio;
        ElementoLista ant_removido = null;
        for (int i = 0; i < pos; i++)
        {
            if (removido == null)
                break;
            ant_removido = removido;
            removido = removido.proximo;
        }
        if (removido != null) {
            if (removido == this.inicio)
                this.inicio = removido.proximo;
            else
                ant_removido.proximo = removido.proximo;
            if (removido == this.fim)
                this.fim = ant_removido;
            
            int tmp = removido.dado;
            
            return tmp;
        }
        else
            return -1;
                
    }
    
    public int buscarValor(int pos)
    {
        ElementoLista atual = this.inicio;
        int i = 0;
        while (atual != null) {
            if (i == pos)
                return atual.dado;
            i++;
            atual = atual.proximo;
        }
        return -1;
    }
    
    
    public int buscarIndice(int valor)
    {
        ElementoLista atual = this.inicio;
        int i = 0;
        while (atual != null) {
            if (atual.dado == valor)
                return i;
            i++;
            atual = atual.proximo;
           
        }
        return -1;
    }
    
    public int tamanhoLista()
    {
        ElementoLista atual = this.inicio;
        int i = 0;
        while (atual != null) {
            i++;
            atual = atual.proximo;
        }
        return i;
    }
    
    public int inserir(int pos, int valor)
    {
        if (pos <= 0)
            this.insereInicio(valor);
        else {
            ElementoLista atual = this.inicio;
            for (int x = 0; x < pos-1; x++) {
                if (atual != null)
                    break;
                atual = atual.proximo;
                
            }
            if (atual == null || atual == this.fim)
                insereFinal(valor);
            else
            {
                ElementoLista novo = new ElementoLista();
                novo.dado = valor;
                novo.proximo = atual.proximo;
                atual.proximo = novo;
                        
                       
            }
        }
        return 0;
    }
}
