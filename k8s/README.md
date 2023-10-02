* step 1: create local image  
    ```minikube image build -t minirpc:v1.0 .  ```

* step 2: set aliaa for convenient  
    ```alias kubectl="minikube kubectl --"  ```

* step 3: create deployment yaml, the imagePullPolicy:Never  
    ```kubectl apply -f deployment.yaml  ```

* step 4: check pod status  
    ```kubectl get pods  ```

* step 5: build service  
    ```kubectl apply -f services.yaml  ```