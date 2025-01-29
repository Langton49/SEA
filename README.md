# SEA WEB BROWSER 🔎
# Table of Contents 📑
1. [Overview](#overview)
2. [DEMO](#demo)
3. [How I Made It](#how-i-made-it)
   1. [Technologies Stack](#tech-used)
4. [How To Run It](#how-to-use)
   1. [Prerequisites](#prerequisites)
   2. [Installation](#installation)
5. [Contributing](#contributing)
6. [Contact](#contact)

## Overview 📃 <a name="overview">
SEA web browser is a desktop application that allows users to surf the internet. It was built using the QT Framework and C++. It utilizes the QWebEngine module (derived from the Chromium browser) to get web pages from URLs. The goal of this browser is to create a safe and private browsing experience.

## DEMO 🎞 <a name="demo">
\
![alt text](https://github.com/Langton49/SEA/blob/master/Demo/00-21-12.gif "SEA Web Browser in action.")

GIF Made With: [Free Convert](https://www.freeconvert.com/)

## How I Made It 🔨 <a name="how-i-made-it">
#### Technology Stack <a name="tech-used">
- QT Framework (Version 6.7.2)
- C++ (Version 13.2.0)
- QT modules (QWebEngine, QtNetwork, etc.)

#### Browsing 💻
To emulate modern browsers, the SEA Web Browser provides users with flexibility when looking up web pages or performing searches. In modern browsers like Chrome or Microsoft Edge, the address bar can either:

1. Navigate to a specified URL.
2. Default to a search using the browser's default search engine (e.g., Google for Chrome, Bing for Edge).

To achieve this functionality, I used a QLineEdit widget to capture user input. Once the user enters a value, the program processes the string to determine if it represents:

- A valid URL: If the input is a valid URL, the program loads and displays the corresponding webpage in the QWebEngineView.
- A domain name: If the input is a domain name, the program restructures it into a valid URL and loads the corresponding page.
- A search query: If the input doesn't match a valid URL or domain name, the program defaults to a Google search.

Additionally, if a valid URL is provided but cannot be resolved (e.g., 404 error), the browser will display an error message instead.

#### Multi-Tab Functionality
Modern browsers allow users to open multiple tabs, each displaying its own webpage and loading separate URLs. However, Qt Creator does not provide a default widget that fully emulates this behavior. While QTabWidget allows switching between different tabs, it lacks dynamic content loading. To overcome this limitation, I created a custom class, which extends QTabWidget and provides a fully functional multi-tab browsing experience. The implementation includes:

##### Tab Management
- A "New Tab" button, positioned in the corner of the tab bar that allows users to open new tabs dynamically.
- Tabs are closable, and the browser closes if no tabs remain.
- Each tab is equipped with a QWebEngineView instance to load and display web content.

##### Tab Icons, Titles and Loading Indicators:
- Tabs initially display a default favicon.
- A loading animation is shown when a webpage is being loaded.
- If a webpage has a favicon, it updates the tab icon accordingly.
- If a URL fails to resolve (e.g., a 404 error), a fallback icon is displayed.
- The browser updates the tab title based on the page title retrieved from QWebEngineView.

##### Window Control Buttons
To implement a clean and modern UI, I created custom window control buttons that match the theme and look of the browser window:
- The interface includes custom minimize, maximize, and close buttons for window management.
- The maximize button toggles between full-screen and normal mode.

The custom tab widget is one of the things I was really proud to have achieved while completing this project as it shows the amazing capabilities of the QT framework and object-oriented programming.

## How To Run It 🤷‍♂️<a name="how-to-use">
### Prerequisites <a name="prerequisites">
- QT (Version 6 or later)
- QT Creator (recommended) (Version 14.0.0)
- Any suitable C++ Compiler supporting C++11/14/17, (Project used MSVC2022 17.9.34728.123)
- C++ (C++11 minimum, C++14 or C++17 recommended)

### Installation <a name="installation">
1. Clone the repository by running in the terminal:
```bash
git clone https://github.com/Langton49/SEA.git
```
2. Open Qt Creator and navigate to the cloned project folder. Select and open the .pro file to load the project.
3. In Qt Creator, go to the Project Settings tab on the left.
4. Click on Manage Kits near the top left of the Project Settings window.
5. Choose and set the compiler you want to use for building the project.
7. Build and run the project by clicking the green Play button near the bottom left of the screen or pressing Ctrl+R.  **TIP:** Switch to the Release version when building and running the project for faster page loading times during execution.
8. Type into the search bar any webpage you would like to visit, whether it is a URL or a domain name and the browser will navigate there.

## Contributing 🤝<a name="contributing">
Because software development is an ongoing and collaborative process, contributions are welcome. Simply create a branch, commit your changes, push to the branch and open a pull request.

## Contact ✉<a name="contact">
__Author:__ Munashe Mukweya\
__Email:__ munashemukweya2022@gmail.com\
__GitHub:__ https://github.com/Langton49

