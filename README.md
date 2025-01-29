# SEA WEB BROWSER 🖥
# Table of Contents 📑
1. [Overview](#overview)
2. [DEMO](#demo)
3. [How I Made It](#how-i-made-it)
4. [Technologies Used](#tech-used)
5. [How To Run It](#how-to-use)
   1. [Prerequisites](#prerequisites)
   2. [Installation](#installation)
6. [Contributing](#contributing)
7. [Contact](#contact)

## Overview 📃 <a name="overview">
SEA web browser is a Windows application that allows users to surf the internet. It was built using the QT Framework and C++. It utilizes the QWebEngine module (derived from the Chromium browser) to get web pages from URLs. The goal of this browser is to create a safe and private browsing experience.

## DEMO 🎞 <a name="demo">
\
![alt text](https://github.com/Langton49/SEA/blob/master/Demo/00-21-12.gif "SEA Web Browser in action.")

GIF Made With: [Free Convert](https://www.freeconvert.com/)

## How I Made It 🔨 <a name="how-i-made-it">
#### Browsing 💻
To emulate modern browsers, the SEA Web Browser provides users with flexibility when looking up web pages or performing searches. In modern browsers like Chrome or Microsoft Edge, the address bar can either:

1. Navigate to a specified URL.
2. Default to a search using the browser's default search engine (e.g., Google for Chrome, Bing for Edge).

To achieve this functionality, I used a QLineEdit widget to capture user input. Once the user enters a value, the program processes the string to determine if it represents:

- A valid URL: If the input is a valid URL, the program loads and displays the corresponding webpage in the QWebEngineView.
- A domain name: If the input is a domain name, the program restructures it into a valid URL and loads the corresponding page.
- A search query: If the input doesn't match a valid URL or domain name, the program defaults to a Google search.

Additionally, if a valid URL is provided but cannot be resolved (e.g., 404 error), the browser will display an error message instead.
\


📑 Multi-Tab functionality\
🧺 Browser caching

## Technologies Used <a name="tech-used">
- QT Framework (Version 6.7.2)
- C++ for core functionality
- QT modules

## How To Run It 🤷‍♂️<a name="how-to-use">
### Prerequisites <a name="prerequisites">
- QT Creator (Version 6.7.2 or later)
- C++ Compiler, (preferably MSVC2019 or later)

### Installation <a name="installation">
1. Clone this repository:
```bash
git clone https://github.com/Langton49/SEA.git
```
2. Open the project in QT Creator by opening the .pro file found in the folder.
3. Set the compiler in the Project Settings Tab on the left.
4. Build and run the project by pressing the green play button near the bottom left of the screen or pressing Ctrl+R.  **TIP:** Switch to the Release version when building and running the project for faster page loading times.

Type into the search bar any webpage you would like to visit, whether it is a URL or a domain name and the browser will take you there. If the domain or URL cannot be resolved, the browser will default to a Google search.

## Contributing 🤝<a name="contributing">
Because software development is an ongoing and collaborative process, contributions are welcome. Simply create a branch, commit your changes, push to the branch and open a pull request.

## Contact ✉<a name="contact">
__Author:__ Munashe Mukweya\
__Email:__ munashemukweya2022@gmail.com\
__GitHub:__ https://github.com/Langton49

