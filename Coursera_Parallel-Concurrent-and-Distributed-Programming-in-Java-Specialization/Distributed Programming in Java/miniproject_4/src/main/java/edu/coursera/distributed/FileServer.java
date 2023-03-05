package edu.coursera.distributed;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * A basic and very limited implementation of a file server that responds to GET
 * requests from HTTP clients.
 */
public final class FileServer {
    /**
     * Main entrypoint for the basic file server.
     *
     * @param socket Provided socket to accept connections on.
     * @param fs     A proxy filesystem to serve files from. See the PCDPFilesystem
     *               class for more detailed documentation of its usage.
     * @param ncores The number of cores that are available to your
     *               multi-threaded file server. Using this argument is entirely
     *               optional. You are free to use this information to change
     *               how you create your threads, or ignore it.
     * @throws IOException If an I/O error is detected on the server. This
     *                     should be a fatal error, your file server
     *                     implementation is not expected to ever throw
     *                     IOExceptions during normal operation.
     */
    public void run(final ServerSocket socket, final PCDPFilesystem fs, final int ncores) throws IOException {
        /*
         * Enter a spin loop for handling client requests to the provided
         * ServerSocket object.
         */
        while (true) {
            // TODO 1) Use socket.accept to get a Socket object
            Socket s = socket.accept();
            Thread thread = new Thread(() -> {
                try {
                    InputStreamReader inputStreamReader = new InputStreamReader(s.getInputStream());
                    BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

                    String[] request = bufferedReader.readLine().trim().split(" ");
                    if (request[0].equals("GET")) {
                        String content = fs.readFile(new PCDPPath((request[1])));
                        OutputStreamWriter writer = new OutputStreamWriter(s.getOutputStream());
                        String response;
                        if (content == null) {
                            response = "HTTP/1.0 404 Not Found\r\n" +
                                    "Server: FileServer\r\n" +
                                    "\r\n";
                        } else {
                            response = "HTTP/1.0 200 OK\r\n" +
                                    "Server: FileServer\r\n" +
                                    "\r\n" +
                                    content +
                                    "\r\n";
                        }
                        writer.write(response);
                        writer.close();
                    } else {
                        socket.close();
                    }
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            });
            thread.start();
        }
    }
}
