# 🖥️ Lexical Analyzer & Symbol Table Generator in C  

![Language](https://img.shields.io/badge/Language-C-blue)  
![Compiler](https://img.shields.io/badge/Compiler-GCC-green)  
![Status](https://img.shields.io/badge/Status-Working-success)  
![License](https://img.shields.io/badge/License-MIT-yellow)  

---

## 📖 Overview  

This project is a **Lexical Analyzer written in C** that:  
- Performs **tokenization** of C source code  
- Extracts **keywords, identifiers, numbers, strings, and symbols**  
- Builds a **symbol table** for declared variables  
- Enforces a **custom variable naming rule (SSSD Rule)**  

The analyzer processes code from an `input.txt` file and writes tokens & symbol table results into an `output.txt` file.  

---

## 🧮 SSSD Variable Naming Rule  

A variable is considered **valid** if:  
- Length = **4 characters**  
- First **3 characters are letters**  
- Last **1 character is a digit**  

✅ Example: `abc1`, `var9`  
❌ Invalid: `abcd`, `12ab`, `a_b1`  

---

## 📂 Project Structure  
lexical-analyzer/
├── lexical_analyzer.c # Main C program
├── input.txt # Input source code (to analyze)
├── output.txt # Output tokens & symbol table

## 🔍 Features

✅ Tokenizes keywords, identifiers, symbols, strings, and numbers

✅ Builds a symbol table for variables & arrays

✅ Enforces SSSD variable rule for identifiers

✅ Handles arrays and strings gracefully

✅ Writes structured output to a file

## ⚠️ Limitations

Only supports basic C code snippets

No support for functions, nested scopes, or complex declarations

Variable scope is currently fixed to global
---

## ⚙️ Compilation & Execution  

1. **Compile the program**  
   ```bash
   gcc lexical_analyzer.c -o lexical_analyzer

