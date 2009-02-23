package cms.dqm.workflow;

import oracle.jdbc.pool.OracleDataSource;
import org.jasypt.util.text.BasicTextEncryptor;
import java.sql.*; 
import java.util.*; 
import javax.naming.*;

public class DBWorker {

  private Connection conn;
  private Encoder enc;

  public DBWorker() throws Exception {
    this(false);
  }

  public DBWorker(String url, String auth) throws Exception {
    this(url, auth, false);
  }

  public DBWorker(String url, String auth, boolean readOnly) throws Exception {
    enc = new Encoder();
    OracleDataSource ds = new OracleDataSource();
    ds.setConnectionCachingEnabled(true);
    ds.setURL(url);
    if (ds.getUser() == null || ds.getUser().equals("")) {
      ds.setUser(enc.getUser(auth));
      ds.setPassword(enc.getPassword(auth));
    }
    connect(ds, readOnly);
  }

  public DBWorker(boolean readOnly) throws Exception {
    enc = new Encoder();
    Context initContext = new InitialContext();
    Context envContext = (Context) initContext.lookup("java:/comp/env");
    if (envContext == null) throw new NamingException("Error: No Context");
    OracleDataSource ds = (OracleDataSource) envContext.lookup(WebUtils.GetEnv("db_name"));
    ds.setConnectionCachingEnabled(true);

    if (ds == null) throw new Exception("Error: No DataSource");

    if (ds.getUser() == null || ds.getUser().equals("")) {
      String auth = WebUtils.GetEnv("db_auth");
      ds.setUser(enc.getUser(auth));
      ds.setPassword(enc.getPassword(auth));
    }

    connect(ds, readOnly);
  }

  private void connect(OracleDataSource ds, boolean readOnly) throws Exception {
    conn = ds.getConnection();
    if (conn == null) throw new SQLException("Error: No Connection");
    conn.setReadOnly(readOnly);
  }

  public void setAutoCommit(boolean value) throws SQLException {
    conn.setAutoCommit(value);
  }

  public void commit() throws SQLException {
    conn.commit();
  }

  public void rollback() throws SQLException {
    conn.rollback();
  }

  public ResultSet executeSQL(String sql) throws SQLException {
    Statement stmt = conn.createStatement();
    return stmt.executeQuery(sql);
  }

  public PreparedStatement prepareSQL(String sql) throws SQLException { 
    PreparedStatement pstmt = conn.prepareStatement(sql);
    return pstmt;
  }

  public boolean executeSQLSilent(String sql, int parameter) throws SQLException {
    PreparedStatement pstmt = prepareSQL(sql);
    pstmt.setInt(1, parameter);
    return pstmt.execute();
  }

  public int getFirstRowValueInt(String sql, int parameter, int fieldnr) throws SQLException {
    PreparedStatement pstmt = prepareSQL(sql);
    pstmt.setInt(1, parameter);
    ResultSet res = pstmt.executeQuery();
    res.next();
    int i = res.getInt(fieldnr);
    res.close();
    return i;
  }

  public int getFirstRowValueInt(PreparedStatement pstmt) throws SQLException {
    ResultSet res = pstmt.executeQuery();
    res.next();
    int i = res.getInt(1);
    res.close();
    return i;
  }

  public String getFirstRowValueString(String sql, int parameter, int fieldnr) throws SQLException {
    PreparedStatement pstmt = prepareSQL(sql);
    pstmt.setInt(1, parameter);
    ResultSet res = pstmt.executeQuery();
    res.next();
    String s = res.getString(fieldnr);
    res.close();
    return s;
  }

  public Vector<String> getStringList(String sql, int fieldnr) throws SQLException {
    Vector<String> list = new Vector<String>();
    ResultSet r = executeSQL(sql);
    while (r.next()) list.add(r.getString(fieldnr));
    r.close();
    return list;
  }

  public Hashtable<String, String> getStringsMap(PreparedStatement pstmt) throws SQLException {
    Hashtable<String,String> list = new Hashtable<String,String>();
    ResultSet r = pstmt.executeQuery();
    while (r.next()) {
      list.put(r.getString(1), r.getString(2));
    }
    r.close();
    return list;
  }

  public void close() {
    try {
      conn.close();
    } catch (Exception e) { }
  }

  protected void finalize() throws Throwable {
    close();
  }

  public static void setStringNull(PreparedStatement pstmt, String value, int number) throws SQLException {
    if (value == null) {
      pstmt.setNull(number, java.sql.Types.VARCHAR); 
    } else {
      pstmt.setString(number, value); 
    }
  }


}
