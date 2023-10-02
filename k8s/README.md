* step 1: create local image  
    ```minikube image build -t minirpc:v1.0 .  ```

* step 2: set alias for convenient  
    ```alias kubectl="minikube kubectl --"  ```

* step 3: create deployment yaml, the imagePullPolicy:Never  
    ```kubectl apply -f deployment.yaml  ```

* step 4: check pod status  
    ```kubectl get pods  ```

* step 5: build service  
    ```kubectl apply -f services.yaml  ```

* step 6: minikube start service  
    ```minikube service minirpc --url  ```

* step 7: choose online tool to test WebSocket
    ```ws://127.0.0.1:52358```