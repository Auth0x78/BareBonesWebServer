# BareBonesWebServer

A minimalistic web server focused on simplicity and lightweight HTTP handling using Winsock2 in C++. This server is designed to serve basic HTML, CSS, JavaScript, and image files with minimal overhead.

## Features

- Lightweight HTTP server built with Winsock2.
- Supports serving static files like HTML, CSS, JS, and images.
- Handles basic GET requests.

## Requirements

- Windows OS
- Winsock2 (Included with Windows SDK)
- Visual Studio (for building the project)

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/YourUsername/BareBonesWebServer.git
    ```
2. Navigate into the cloned directory and open the `BareBonesWebServer.sln` file in **Visual Studio**.

3. Build the project:
    - Select the **Build** menu.
    - Click **Build Solution**.

## Usage

1. After building, run the server from the Visual Studio environment or by executing the `.exe` file from the build directory:
    ```bash
    ./BareBonesWebServer.exe
    ```

2. Open a browser and navigate to:
    ```
    http://localhost:8080
    ```

3. To serve additional static files, place them in the '*pages*' directory as the server or modify the code to handle more routes and file types.

## Example

- A client can access:
    - `index.html`: For the home page.
    - Other static files like CSS, JS, and images.

## Contributing

Feel free to fork this repository and submit pull requests for new features, bug fixes, or enhancements.

## License
[LICENSE.md]
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
