# Install protobuf:

```
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf
git checkout v3.9.0
git submodule update --init --recursive
./autogen.sh

./configure
make
make check
sudo make install
sudo ldconfig
```

# Generate header and source file:

Go to the directory 
```
cd src/3rdparty/chromium/third_party/blink/renderer/core/html/jimber/protobuf
```

Run the following command:
```
protoc -I=. --cpp_out=cpp/ ./*.proto
```
