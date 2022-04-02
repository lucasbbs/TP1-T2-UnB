#include "entidades.h"
#include "dominios.h"
#include "sqlite3.h"
#include "database.h"
#include <string>
#include <sstream>

//Atributo estatico container List.

list<ElementoResultado> ComandoSQL::listaResultado;

//---------------------------------------------------------------------------
//Classe ErroPersistencia.

EErroPersistencia::EErroPersistencia(string mensagem)
{
        this->mensagem = mensagem;
}

string EErroPersistencia::what()
{
        return mensagem;
}

//---------------------------------------------------------------------------
//Classe ElementoResultado.

void ElementoResultado::setNomeColuna(const string &nomeColuna)
{
        this->nomeColuna = nomeColuna;
}

void ElementoResultado::setValorColuna(const string &valorColuna)
{
        this->valorColuna = valorColuna;
}

//---------------------------------------------------------------------------
//Classe ComandoSQL.

void ComandoSQL::conectar()
{
        rc = sqlite3_open(nomeBancoDados, &bd);
        if (rc != SQLITE_OK)
                throw EErroPersistencia("Erro na conexao ao banco de dados");
}

void ComandoSQL::desconectar()
{
        rc = sqlite3_close(bd);
        if (rc != SQLITE_OK)
                throw EErroPersistencia("Erro na desconexao ao banco de dados");
}

void ComandoSQL::executar()
{
        conectar();
        rc = sqlite3_exec(bd, comandoSQL.c_str(), callback, 0, &mensagem);

        if (rc != SQLITE_OK)
        {
                sqlite3_free(mensagem);
                desconectar();
                throw EErroPersistencia("Erro na execucao do comando SQL");
        }

        desconectar();
}

int ComandoSQL::callback(void *NotUsed, int argc, char **valorColuna, char **nomeColuna)
{
        NotUsed = 0;
        ElementoResultado elemento;
        int i;
        for (i = 0; i < argc; i++)
        {
                elemento.setNomeColuna(nomeColuna[i]);
                elemento.setValorColuna(valorColuna[i] ? valorColuna[i] : "NULL");
                listaResultado.push_front(elemento);
        }
        return 0;
}

ComandoSenha::ComandoSenha(Email email)
{
        comandoSQL = "SELECT senha FROM usuario WHERE email = '";
        // senha da tabela usuario onde email eh o getemail
        comandoSQL += email.getEmail();
        comandoSQL += "'";
}

string ComandoSenha::getResultado()
{
        ElementoResultado resultado;
        string senha;

        if (listaResultado.empty())
        {
                throw EErroPersistencia("Lista Vazia.");
        }

        resultado = listaResultado.back();
        listaResultado.pop_back(); // dado que a gente vai excluir

        senha = resultado.getValorColuna();

        return senha;
}

// ---------------------------------------------
// Implementação do comando Cadastrar Usuario
// ---------------------------------------------
ComandoCadastrarUsuario::ComandoCadastrarUsuario(Usuario usuario)
{
        comandoSQL = "INSERT INTO usuario VALUES (";
        comandoSQL += "'" + usuario.getEmail().getEmail() + "', ";
        comandoSQL += "'" + usuario.getNome().getNome() + "', ";
        comandoSQL += "'" + usuario.getSenha().getSenha() + "')";
}

// ---------------------------------------------
// Implementação do comando Descadastrar Usuario
// ---------------------------------------------
ComandoDescadastrarUsuario::ComandoDescadastrarUsuario(Email email)
{
        comandoSQL = "DELETE FROM usuario WHERE email = '";
        comandoSQL += email.getEmail();
        comandoSQL += "'";
}

// ---------------------------------------------
// Implementação do comando Editar Usuário
// ---------------------------------------------

ComandoEditarUsuario::ComandoEditarUsuario(Usuario usuario)
{
        comandoSQL = "UPDATE usuario ";
        comandoSQL += "SET nome = '" + usuario.getNome().getNome();
        comandoSQL += "', senha = '" + usuario.getSenha().getSenha();
        comandoSQL += "' WHERE email = 'lucasbbs@live.fr'"; //+ usuario.getEmail().getEmail();
}

//ComandoPesquisarUsuario::ComandoPesquisarUsuario(Email email) {}

//--------------------------------------------------------------------------------------
//|                                 Excursão                                           |
//--------------------------------------------------------------------------------------

// ------------------------------------------------
// Implementação do comando Cadastrar Excursão
// ------------------------------------------------
ComandoCadastrarExcursao::ComandoCadastrarExcursao(Excursao excursao)
{
        comandoSQL = "INSERT INTO Excursao VALUES (";
        comandoSQL += "'" + excursao.getCodigo().getCodigo() + "', ";
        comandoSQL += "'" + excursao.getTitulo().getTitulo() + "', ";
        comandoSQL += "'" + to_string(excursao.getNota().getNota()) + "', ";
        comandoSQL += "'" + excursao.getCidade().getCidade() + "', ";
        comandoSQL += "'" + excursao.getDuracao().getDuracao() + "', ";
        comandoSQL += "'" + excursao.getEndereco().getEndereco() + "', ";
        comandoSQL += "'" + excursao.getDescricao().getDescricao() + "')";
}

NextIdExcursao::NextIdExcursao()
{
        comandoSQL = "SELECT seq + 1 FROM SQLITE_SEQUENCE WHERE name='Excursao'";
}
int NextIdExcursao::getResultado()
{
        ElementoResultado resultado;
        int result;

        if (listaResultado.empty())
        {
                throw EErroPersistencia("Lista Vazia.");
        }
        resultado = listaResultado.back();
        listaResultado.pop_back();

        result = stoi(resultado.getValorColuna());
        return result;
}

// ------------------------------------------------
// Implementação do comando Descadastrar Excursão
// ------------------------------------------------
ComandoDescadastrarExcursao::ComandoDescadastrarExcursao(Codigo codigo)
{
        comandoSQL = "DELETE FROM Excursao WHERE (Codigo = '";
        comandoSQL += codigo.getCodigo();
        comandoSQL += "') AND (Guia = '";
        comandoSQL += "lucasbbs@live.fr"; //email.getEmail();
        comandoSQL += "')";
}

// ------------------------------------------------
// Implementação do comando Editar Excursão
// ------------------------------------------------

ComandoEditarExcursao::ComandoEditarExcursao(Excursao excursao)
{
        comandoSQL = "UPDATE Excursao ";
        comandoSQL += "SET Titulo = '" + excursao.getTitulo().getTitulo();
        comandoSQL += "', Nota = " + to_string(excursao.getNota().getNota());
        comandoSQL += ", Cidade = '" + excursao.getCidade().getCidade();
        comandoSQL += "', Duracao = " + excursao.getDuracao().getDuracao();
        comandoSQL += ", Descricao = '" + excursao.getDescricao().getDescricao();
        comandoSQL += "', Endereco = '" + excursao.getEndereco().getEndereco();
        comandoSQL += "' WHERE (Codigo = '" + excursao.getCodigo().getCodigo();
        comandoSQL += "') AND (Guia = 'lucasbbs@live.fr')";
}

// ------------------------------------------------
// Implementação do comando Listar Excursões
// ------------------------------------------------

ComandoListarExcursoes::ComandoListarExcursoes()
{
        comandoSQL = "SELECT * FROM Excursao";
}