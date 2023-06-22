# Choose a base image that supports C++
FROM gcc:latest

# Copy the C++ executable to the container
COPY Program ${folder}/Program 

# Set the working directory
WORKDIR /app

# Set the entry point to run the executable
ENTRYPOINT ["./Program"]