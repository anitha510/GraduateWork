package example.spark.reviewcloud.driver


import java.util
import java.util.Properties

import edu.stanford.nlp.ling.{CoreAnnotations, CoreLabel}
import edu.stanford.nlp.pipeline.{Annotation, StanfordCoreNLP}
import org.apache.lucene.analysis.StopAnalyzer
import org.apache.spark.ml.feature.StopWordsRemover
import org.apache.spark.rdd.RDD
import org.apache.spark.{SparkConf, SparkContext}

import scala.collection.JavaConversions._
import scala.collection.immutable.ListMap

object SparkAction {

  def main(args: Array[String]): Unit = {
    val conf = new SparkConf().setAppName("review-cloud")
    conf.setMaster("local[*]") //comment this line for cloud
    val sc = new SparkContext(conf)
    val pressData: RDD[(String, String)] = sc.wholeTextFiles("C:/Development/eclipse/workspace/SparkProject/input/*/", 4) //("s3://senatorbucket/input/*/", 4)
    val mappedData: RDD[(String, String)] = pressData.map { case (key, value) => {
      val dirs: Array[String] = key.split("/")
      val author = dirs(dirs.length - 2)
      (author.toLowerCase, value)
    }
    }

    val authorGroup: RDD[(String, String)] = mappedData.reduceByKey(_ + _)

    val stopWords = new StopWordsRemover().setCaseSensitive(true)

    val stopWordsBag = stopWords.getStopWords.toList;

    val toPrint: RDD[(String, util.List[String])] = authorGroup.mapValues(value => {
      val data: util.ArrayList[String] = this.stopWords(value)
      data
    })

    val finalData : RDD[(String, String)]  = toPrint.mapValues(data => {

      val countMap : Map[String, Int] = data.foldLeft(Map.empty[String, Int]) {
        (count, word) => count + (word -> (count.getOrElse(word, 0) + 1))
      }

      val sortedMap : Map[String, Int] = ListMap(countMap.toSeq.sortWith(_._2 > _._2):_*).take(20)

      (sortedMap.keys).mkString(",")
    })

    finalData.saveAsTextFile("C:/Development/eclipse/workspace/SparkProject/output") //("s3://senatorbucket/output/")

  }


  def stopWords(input: String): util.ArrayList[String] = {
    val props = new Properties()
    props.put("annotators", "tokenize, ssplit, stopword");
    props.setProperty("customAnnotatorClass.stopword", "example.learning.annotator.StopwordAnnotator");
    val pipeline: StanfordCoreNLP = new StanfordCoreNLP(props)
    val document: Annotation = new Annotation(input)
    pipeline.annotate(document)
    val tokens: List[CoreLabel] = document.get(classOf[CoreAnnotations.TokensAnnotation]).toList
    val data: util.ArrayList[String] = new util.ArrayList[String]()
    val stopWordsBag = StopAnalyzer.ENGLISH_STOP_WORDS_SET
    tokens.foreach(token => {
      val tokenString = token.word().toLowerCase
      if (!stopWordsBag.contains(tokenString)) {
        data.add(tokenString)
      }
    })
    data
  }

}
