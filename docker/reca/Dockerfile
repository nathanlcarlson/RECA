# Parent image
FROM mongocxxlocal

WORKDIR /tmp
RUN mkdir /root/.ssh
RUN ssh-keyscan -H github.com >> ~/.ssh/known_hosts
RUN git clone https://github.com/nathanlcarlson/RECA.git

WORKDIR /tmp/RECA

RUN git checkout mongo
RUN make reca

# Set the working directory to /app
RUN mkdir /app
RUN cp reca /app

WORKDIR /app
# Copy the current directory contents into the container at /app
ADD ./requirements.txt /app
ADD ./app.py /app
ADD ./templates/hello.html /app/templates/

# Install any needed packages specified in requirements.txt
RUN pip install -r requirements.txt

# Make port 80 available to the world outside this container
EXPOSE 80

# Define environment variable
ENV NAME World

# Run app.py when the container launches
CMD ["python", "app.py"]

# Run app.py when the container launches
#CMD ["/tmp/RECA/reca", "db", "10", "1", "0", "100"]