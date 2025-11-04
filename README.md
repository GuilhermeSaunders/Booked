# 📚 **Booked**

**Booked** é uma plataforma colaborativa que permite que pessoas **compartilhem e aluguem livros, videogames, jogos de tabuleiro e filmes entre si**, resgatando o prazer de explorar histórias e mundos reais sem precisar comprar tudo.

Nosso objetivo é tornar o acesso à cultura:

✅ **Mais fácil**  
✅ **Mais econômico**  
✅ **Mais sustentável**

Conectamos pessoas com interesses em comum e fortalecemos o senso de comunidade.  
No Booked, qualquer pessoa pode disponibilizar itens da própria coleção e ganhar dinheiro com isso — enquanto outros descobrem novas obras pagando muito menos do que pagariam para comprar.

> 🎯 *Mais acesso, mais cultura, mais conexão entre pessoas.*

---

## ⚙️ **Como executar o projeto em Linux e Mac**

### 1️⃣ Clone o repositório
```bash
git clone https://github.com/GuilhermeSaunders/Booked
```
### 2️⃣ Acesse o diretório
```bash
cd Booked
```

### 3️⃣ Compile o código
```bash
g++ -std=c++17 -I./src -I./src/BusinessRules -I./src/Customer -I./src/Product \src/*.cpp src/BusinessRules/*.cpp src/Customer/*.cpp src/Product/*.cpp -o booked_system -lsqlite3
```
### 4️⃣ Execute o programa
```bash
./booked_system
```
### 🧩 Conceitos Utilizados
* **POO (Programação Orientada a Objetos)**: encapsulamento, herança, polimorfismo.
* **CRUD**: gerenciamento de dados (Create, Read, Update, Delete).
* **Arquitetura modular**: separação entre entidades (usuários e produtos), regras de negócio (login, validação, aluguel) e persistência (repositório e banco de dados).
* **Composição e injeção de dependência**: App controla o fluxo e recebe Repository como dependência.
* **Validação e segurança**: Login e Validate garantem integridade dos dados e autenticação segura.

## Site Oficial do Projeto
* https://guilhermesaunders.github.io/Booked/ 
## Link para vídeo explicativo no Youtube
* https://youtu.be/NlOnni-9TqE?si=DyPHSQa5Jz1Kv7B6 
