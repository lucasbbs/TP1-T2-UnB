#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <bits/stdc++.h>

#include "entidades.h"
#include "dominios.h"


using namespace std;

void Usuario::setUsuario(string novoNome, string novoEmail, string novaSenha){
  nome.setNome(novoNome);
  email.setEmail(novoEmail);
  senha.setSenha(novaSenha);
}

void Avaliacao::setAvaliacao(string novoCodigo, int novaNota, string novaDescricao){
    codigo.setCodigo(novoCodigo);
    nota.setNota(novaNota);
    descricao.setDescricao(novaDescricao);
}
