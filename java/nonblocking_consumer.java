
#### A single app interested in consuming a known topic from beginning (asynchronously)
Method `consumeForeverNonBlocking` is the entry point.

```java
private CompletableFuture<StreamSet> setupConsumptionNonBlocking(String topicName, String namespace) {
    ClientFactory clientFactory = new ClientFactoryImpl();
    TopicConsumer topicConsumer = clientFactory.createTopicConsumer(ConsumerConfigsBuilder().foo().bar().build());
    Topic topic = new Topic(topicName, namespace); 
    topicConsumer.describeRangesFor(Collections.singleton(topic))
                 .thenCompose((RangeDescription rangeDescription) -> {
                     List<Range> oldestRanges = rangeDescription.get(topic).get(0); 
                     Set<RangePosition> positions = new Set<>();
                     for(Range range: oldestRanges) {
                       positions.add(RangePosition.beginning(range));
                     }
                     return topicConsumer.createStreamSet().subscribe(positions);
                 })
                 .thenCompose((SubscriptionResult result) -> {
                     result.forEach((rangePosition, result) -> { assert result.isSuccess(); });
                     return result.streamSet();
                 });
}

private CompletableFuture<StreamSet> continueConsumeNonBlocking(StreamSet streamSet) {
    return streamSet.poll(42).thenComposeAsync((ConsumerRecords records) -> {
      log.info("Received records");
      return continueConsumeNonBlocking(streamSet);
    });
}

public CompletableFuture<Void> consumeForeverNonBlocking(String topicName, String namespace) {
  return setupConsumptionNonBlocking(topicName, namespace)
             .thenAccept((StreamSet streamSet) -> {
                 continueConsumeNonBlocking(streamSet);
             });
}
```
